//
// Created by YING JIEH XIA on 2024/5/5.
//

#ifndef ECE141DB_SELECTSTATEMENT_HPP
#define ECE141DB_SELECTSTATEMENT_HPP

#include "Statement.hpp"
#include "database/Database.hpp"
#include "parsing/SelectParser.hpp"
#include "database/RowSelection.hpp"
#include "misc/joins/Join.hpp"

namespace ECE141 {
    class SelectStatement : public Statement {
    public:
        SelectStatement (SelectParser& aSelectParser) : selectParser(aSelectParser) {}
        StatusResult parse(TokenParser& tokenParser) override {
            return tokenParser.clear().parseWith(selectParser).getState();
        }

        StatusResult run(ExecutionContext& aContext) override {
            INFO("SelectStatement start running")
            DBQuery& query = selectParser.theQuery;
            query.resolveTablesAndFields(aContext);

            for (Table* table : query.table) {
                table->loadAllRows(
                        [&](const TableRow& aRow){
                            // TODO: Insert Table Specific Filter Logic
                            // (Only applies to one filter row)
                            // TODO: For now, just consider ALL to be AND
                            return query.matches(aRow);
                        },
                        [&](const IdentifierHash aHash){
                            // TODO: Insert any filters that only apply to the IdentifierHash
                            // Note: This is an optimization, not strictly necessary
                            return true;
                        }
                );
            }

            if (query.table.size() == 1 && query.getAll() && query.groupByFields.empty() && query.limits.size() < 2
                && query.orderByFields.empty() && query.getFilterExpressions().empty()) {
                // Simple query with ONLY LIMIT and *
                TabularView simpleView{*query.table[0]};
                if (!query.limits.empty()) simpleView.setLimit(query.limits[0]);
                aContext.viewListener(simpleView);
                return {Errors::noError, query.table[0]->cache.requiredSize()};
            }

            if (query.getAll()) {
                for (Table* table : query.table) {
                    table->attributes.orderedEach([&](size_t idx, const std::string& name, const auto& _){
                        std::string alias = query.table.size() > 1 ? (table->name + "." + name) : name;
                        // Qualify the alias with the table name if joining multiple tables
                        query.selectedFields.push_back({name, table->name, alias});
                        return true;
                    });
                }
            }
            RowSelection selection{query.selectedFields, query.table};
            std::shared_ptr<Join> join = joinFactory(query);
            join->each([&](const Row& aRow) {
                selection.add(const_cast<Row *>(&aRow));
                return true;
            });
//            // TODO: Join logic...
//            // For now assume we just have one table, later, we will change this to iterate over all JOINED rows
//            Table* table = query.table[0];
//            table->cache.eachRequired([&](const IdentifierHash& id, const TableRow& row, const CacheStatus& _){
//                // TODO: Add joined row level filter logic (eg if(Filter(Row)) selection.add(id))
//
//                selection.add((Row *) &row); // For now add all rows to selection
//                return true;
//            });

            returnIfError(selection.orderBy(query.orderByFields));
            returnIfError(selection.limit(query.limits));

            TabularView view{&selection.outputSchema, &selection};
            aContext.viewListener(view);

            // Kinda cheating by keeping the unbounded cache through multiple queries so clearing it here.
            for (Table* table : query.table) {
                table->cache.clear();
            }

            INFO("SelectStatement finish running")
            return {Errors::noError, selection.size()};
        }


        SelectParser& selectParser;
    };
}




#endif //ECE141DB_SELECTSTATEMENT_HPP
