//
// Created by YING JIEH XIA on 2024/4/21.
//

#ifndef ECE141DB_SQLPROCESSOR_HPP
#define ECE141DB_SQLPROCESSOR_HPP

#include "CommandHandler.hpp"
#include "parsing/TokenParser.hpp"
#include "database/Database.hpp"
#include "view/TabularView.hpp"
#include "parsing/CreateTableParser.hpp"
#include "statement/InsertStatement.hpp"
#include "statement/SelectStatement.hpp"
#include "statement/UpdateStatement.hpp"
#include "statement/DeleteStatement.hpp"

namespace ECE141{
    class SQLProcessor : public CommandHandler {
    public:
        explicit SQLProcessor(ExecutionContext& aContext) : CommandHandler(aContext) {}
        ~SQLProcessor() override = default;
        StatusResult handle(Tokenizer& aTokenizer) override {
            TRACE("SQLProcessor is handling")
            if (aTokenizer.size() < 1) {
                ERROR("Tokenizer.size < 1 in SQLProcessor")
                return Errors::unknownCommand;
            }

            TokenParser theParser{aTokenizer};
            while (aTokenizer.more()) {
                if (theParser.clear().nextIs(createTBKW)) {
                    return createTB(theParser);
                } else if (theParser.clear().nextIs(dropTBKW)) {
                    return dropTB(theParser);
                } else if (theParser.clear().nextIs(showTBsKW)) {
                    return showTBs(theParser);
                } else if (theParser.clear().nextIs({Keywords::describe_kw})) {
                    return describeTB(theParser);
                } else if (theParser.clear().nextIs(insertIntoKW)) {
                    return insert(theParser);
                } else if (theParser.clear().nextIs({Keywords::select_kw})) {
                    return select(theParser);
                } else if (theParser.clear().nextIs({Keywords::update_kw})) {
                    return update(theParser);
                } else if (theParser.clear().nextIs({Keywords::delete_kw})) {
                    return processDelete(theParser);
                }
                TRACE("SQLProcessor cannot handle")
                return Errors::unknownCommand;
            }
            TRACE("SQLProcessor cannot handle")
            return Errors::unknownCommand;
        }

    protected:
        StatusResult createTB(TokenParser& aParser) {
            INFO("Handling create table")
            StatusResult theResult;
            aParser.clear();

            auto theDB = context.database;
            if (!theDB) {
                ERROR("noDatabaseSpecified when creating table")
                theResult.error = Errors::noDatabaseSpecified;
                return theResult;
            }

            CreateTableParser createTableParser{theDB};

            if (!aParser.parseWith(createTableParser)) {
                theResult = aParser.getState();
                if (theResult.error != Errors::noError)
                    return theResult;
            }

            // Try to create a table, if it already exists throw an error.
            // This step also adds the table to the DB's Index, and writes the schema to the file.
            theResult = theDB->createTable(std::move(createTableParser.table));
            theResult.value = 1;
            INFO("Successfully handled create table")
            return theResult;
        }

        StatusResult dropTB(TokenParser& aParser) {
            INFO("Handling drop table")
            StatusResult theResult;
            std::string theTBName;

            auto theDB = context.database;
            if (!theDB) {
                ERROR("noDatabaseSpecified when dropping table")
                return Errors::noDatabaseSpecified;
            }

            if (!aParser.getIdentifier(theTBName)) {
                ERROR("identifierExpected when dropping table")
                theResult.error = Errors::identifierExpected;
            }
            else {
                std::map<std::string, Table>& tables = theDB->tables;
                TRACE("Get tables map from the database in use")
                auto it = tables.find(theTBName);
                if (it != tables.end()) {
                    theResult.value = it->second.size();
                    tables.erase(it);
                } else {
                    ERROR("unknownTable when dropping table")
                    theResult.error = Errors::unknownTable;
                }
            }
            INFO("Successfully dropping table ", theTBName)
            return theResult;
        }

        StatusResult showTBs(TokenParser& aParser) {
            INFO("Handling show tables")
            StatusResult theResult;

            auto theDB = context.database;
            if (!theDB) {
                ERROR("noDatabaseSpecified when showing tables")
                theResult.error = Errors::noDatabaseSpecified;
                return theResult;
            }

            std::string theTitle = "Tables_in_";
            theTitle.append(theDB->name);
            Table t(theTitle);
            t.addAttributes({{theTitle}});

            for (auto&[name, table] : theDB->tables) {
                TableRow r{&t, {{table.name}}};
                t.addRow(r);
            }

            theResult.value = t.size();
            TabularView tabView{t};
            context.viewListener(tabView);
            return theResult;
        }

        StatusResult describeTB(TokenParser& aParser) {
            INFO("Handling describe table")
            StatusResult theResult;
            std::string theTBName;

            auto theDB = context.database;
            if (!theDB) {
                ERROR("noDatabaseSpecified when describing tables")
                return Errors::noDatabaseSpecified;
            }

            if (!aParser.getIdentifier(theTBName)) {
                ERROR("identifierExpected when describing table")
                theResult.error = Errors::identifierExpected;
                return theResult;
            }
            else {
                auto it = theDB->tables.find(theTBName);
                if (it != theDB->tables.end()) {
                    Table tableToShow = it->second.describe();
                    TabularView theTBView{tableToShow, 2, false};
                    context.viewListener(theTBView);

                    std::stringstream ss;
                    ss << tableToShow.size() << " rows in set ("
                       << std::fixed << theTimer.elapsed(theStart) << " sec)";
                    StringView sv{ss.str()};
                    context.viewListener(sv);
                }

            }
            INFO("Successfully described table ", theTBName)
            return theResult;
        }

        StatusResult insert(TokenParser& aParser) {
            INFO("Handling insert into")
            InsertParser insertParser;
            InsertStatement statement(insertParser);
            StatusResult theResult = statement.parse(aParser);

            if (!context.database) {
                ERROR("noDatabaseSpecified when describing tables")
                return Errors::noDatabaseSpecified;
            }
            if (!theResult) { return theResult; }

            theResult = statement.run(context);
            INFO("Successfully handled insert to ", insertParser.tableName)
            return theResult;
        }

        StatusResult select(TokenParser& aParser) {
            INFO("Handling select table")
            SelectParser selectParser;
            SelectStatement selectStatement(selectParser);
            StatusResult theResult = selectStatement.parse(aParser);

            if (!context.database) {
                ERROR("noDatabaseSpecified when selecting tables")
                return Errors::noDatabaseSpecified;
            }
            if (!theResult) { return theResult; }

            theResult = selectStatement.run(context);
            INFO("Successfully handled select")
            return theResult;
        }

        StatusResult update(TokenParser& aParser) {
            INFO("Handling update table")
            DBQuery query;
            UpdateParser updateParser(query);
            UpdateStatement updateStatement(updateParser);
            StatusResult theResult = updateStatement.parse(aParser);

            if (!context.database) {
                ERROR("noDatabaseSpecified when updating tables")
                return Errors::noDatabaseSpecified;
            }
            if (!theResult) { return theResult; }

            theResult = updateStatement.run(context);
            INFO("Successfully updated table")
            return theResult;
        }

        StatusResult processDelete(TokenParser& aParser) {
            INFO("Handling delete table")
            DBQuery query;
            DeleteParser deleteParser(query);
            DeleteStatement deleteStatement(deleteParser);
            StatusResult theResult = deleteStatement.parse(aParser);

            if (!context.database) {
                ERROR("noDatabaseSpecified when deleting table")
                return Errors::noDatabaseSpecified;
            }
            if (!theResult) { return theResult; }

            theResult = deleteStatement.run(context);
            INFO("Successfully deleted table")
            return theResult;
        }

    protected:
        static constexpr KWList createTBKW{Keywords::create_kw,Keywords::table_kw};
        static constexpr KWList dropTBKW{Keywords::drop_kw,Keywords::table_kw};
        static constexpr KWList showTBsKW{Keywords::show_kw,Keywords::tables_kw};
        static constexpr KWList insertIntoKW{Keywords::insert_kw,Keywords::into_kw};
    };
}


#endif //ECE141DB_SQLPROCESSOR_HPP
