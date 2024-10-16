#ifndef ECE141DB_DATABASEHANDLER_HPP
#define ECE141DB_DATABASEHANDLER_HPP

#include <functional>
#include <map>
#include <fstream>
#include <filesystem>
#include <memory>
#include "CommandHandler.hpp"
#include "controller/AppController.hpp"
#include "misc/Config.hpp"
#include "parsing/TokenParser.hpp"
#include "view/QueryView.hpp"
#include "database/Database.hpp"
#include "misc/FolderReader.hpp"
#include "view/TabularView.hpp"

namespace ECE141 {
    class DatabaseHandler : public CommandHandler {
    public:
        explicit DatabaseHandler(ExecutionContext& aContext) : CommandHandler(aContext) {}
        ~DatabaseHandler() override = default;

        StatusResult handle(Tokenizer& aTokenizer) override {
            TRACE("DatabaseHandler handling")
            if (aTokenizer.size() < 1) {
                ERROR("Tokenizer.size < 1 in DatabaseHandler")
                return Errors::unknownCommand;
            }

            TokenParser theParser{aTokenizer};

            while (aTokenizer.more()) {
                if (theParser.clear().nextIs(createDBKW)) {
                    return createDB(theParser);
                } else if (theParser.clear().nextIs(dropDBKW)) {
                    return dropDB(theParser);
                } else if (theParser.clear().nextIs({Keywords::use_kw})) {
                    return useDB(theParser);
                } else if (theParser.clear().nextIs(showDBsKW)) {
                    return showDB();
                } else if (theParser.clear().nextIs(dumpDBKW)) {
                    return dumpDB(theParser);
                }
                TRACE("DatabaseHandler cannot handle")
                return Errors::unknownCommand;
            }
            TRACE("DatabaseHandler cannot handle")
            return Errors::unknownCommand;
        }

    protected:
        using Callback = std::function<bool(Token&)>;
        using Path = std::filesystem::path;

        std::string folder;

        StatusResult createDB(TokenParser& aParser) {
            INFO("Handling create database")
            StatusResult theResult;
            std::string theDBName;

            if (!aParser.getIdentifier(theDBName)) {
                ERROR("IdentifierExpected when creating database")
                theResult.error = Errors::identifierExpected;
                return theResult;
            }

            if (theResult) {
                std::string theDBPath = Config::getDBPath(theDBName);
                Path pathObj(theDBPath);
                if (std::filesystem::exists(pathObj)) {
                    ERROR("databaseExists when creating database")
                    theResult.error = Errors::databaseExists;
                    return theResult;
                } else {
                    Database theDB(theDBName, CreateFile());
                }
            }

            theResult.value = 1;
            INFO("Successfully created database", theDBName)
            return theResult;
        }

        StatusResult dropDB(TokenParser& aParser) {
            INFO("Handling drop database")
            StatusResult theResult;
            std::string theDBName;
            if (!aParser.getIdentifier(theDBName)) {
                ERROR("unexpectedIdentifier when dropping database")
                theResult.error = Errors::unexpectedIdentifier;
                return theResult;
            }
            std::string theDBPath = Config::getDBPath(theDBName);
            Path pathObj(theDBPath);
            if (!std::filesystem::exists(pathObj)) {
                ERROR("unknownDatabase when dropping database")
                theResult.error = Errors::unknownDatabase;
                return theResult;
            }

            std::filesystem::remove(pathObj);
            theResult.value = 0;
            INFO("Successfully delete database ", theDBName);

            return theResult;
        }

        StatusResult showDB() {
            INFO("Handling show databases")
            StatusResult theResult;
            FolderReader f{Config::getStoragePath()};
            TRACE("Read the storage folder")

            Table t("Databases");
            t.addAttributes({{"Databases"}});

            f.each(".db", [&](const std::string& name){
                std::filesystem::path p{name};
                std::string dbName = p.filename().stem();
                TableRow r{&t, {{dbName}}};
                t.addRow(r);
                TRACE("Added database ", dbName, " to the showing table")
                return true;
            });
            theResult.value = t.size();
            TabularView tabView{t};
            context.viewListener(tabView);
            INFO("Successfully showed databases")
            return theResult;
        }

        StatusResult useDB(TokenParser& aParser) {
            INFO("Handling use database")
            StatusResult theResult;
            std::string theDBName;

            if (!aParser.getIdentifier(theDBName)) {
                ERROR("unexpectedIdentifier when using database")
                theResult.error = Errors::unexpectedIdentifier;
                return theResult;
            }

            std::string theDBPath = Config::getDBPath(theDBName);
            if (!std::filesystem::exists(theDBPath)) {
                ERROR("unknownDatabase when using database")
                theResult.error = Errors::unknownDatabase;
                return theResult;
            }

            context.database = std::make_shared<Database>(theDBName, OpenFile());
            context.database->setSharedPtr(context.database); // TODO: should use shared_from_this() but couldn't get it to work
            std::stringstream text;
            text << "Database changed to " << theDBName << " ("
                << std::fixed << theTimer.elapsed(theStart) << " sec)";

            StringView s{text.str()};
            context.viewListener(s);
            INFO("Successfully used database ", theDBName)
            return theResult;
        }

        StatusResult dumpDB(TokenParser& aParser) {
            INFO("Handling dump database")
            StatusResult theResult;
            std::string theDBName;

            if (!aParser.getIdentifier(theDBName)) {
                ERROR("identifierExpected when dumping database")
                theResult.error = Errors::identifierExpected;
            }

            std::string theDBPath = Config::getDBPath(theDBName);
            if (!std::filesystem::exists(theDBPath)) {
                ERROR("unknownDatabase when dumping database")
                theResult.error = Errors::unknownDatabase;
            }

            Database theDB(theDBName, OpenFile());
            Table t{"Dump"};
            Attribute pk = {"Block", DataTypes::int_type};
            pk.primary_key = true;
            t.addAttribute(pk);
            t.addAttributes({
                    {"Type", DataTypes::varchar_type},
                    {"Hash", DataTypes::int_type},
                    {"Next Block", DataTypes::int_type}
            });

            theDB.storage.each([&](const Block& aBlock, uint32_t index) {
                std::string typeStr{(char)aBlock.header.type};
                TableRow r{&t, {
                        (int)index, typeStr, (int)aBlock.header.hash, (int)aBlock.header.nextBlock
                }};
                t.addRow(r);
                return true;
            });

            TabularView tabularView{t};
            context.viewListener(tabularView);

            theResult.value = t.size();
            INFO("Successfully dumped database", theDBName)
            return theResult;
        }

        std::map<Keywords, Callback> handlerMap = {
                {Keywords::create_kw, [this](Token& token){
                    std::stringstream text;
                    text << "Authors: " << Config::getMembers() << std::endl;
                    StringView s{text.str()};
                    context.viewListener(s);
                    return true;
                }
            }
        };

        static constexpr KWList createDBKW{Keywords::create_kw,Keywords::database_kw};
        static constexpr KWList showDBsKW{Keywords::show_kw,Keywords::databases_kw};
        static constexpr KWList dumpDBKW{Keywords::dump_kw,Keywords::database_kw};
        static constexpr KWList dropDBKW{Keywords::drop_kw,Keywords::database_kw};
    };
}

#endif //ECE141DB_DATABASEHANDLER_HPP
