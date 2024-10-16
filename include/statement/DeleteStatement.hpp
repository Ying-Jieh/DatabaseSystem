//
// Created by Daniel Ziper on 5/12/24.
//

#ifndef ECE141DB_DELETESTATEMENT_HPP
#define ECE141DB_DELETESTATEMENT_HPP

#include "statement/Statement.hpp"
#include "misc/DBQuery.hpp"
#include "database/Table.hpp"
#include "parsing/DeleteParser.hpp"

namespace ECE141 {
    class DeleteStatement : public Statement {
    public:
        explicit DeleteStatement(DeleteParser& anDeleteParser) : deleteParser(anDeleteParser) {}

        StatusResult parse(TokenParser &tokenParser) override {
            return tokenParser.clear().parseWith(deleteParser).getState();
        }

        StatusResult run(ExecutionContext &aContext) override {
            INFO("DeleteStatement start running")
            DBQuery& query = deleteParser.theQuery;
            returnIfError(query.resolveTablesAndFields(aContext));
            Table* table = query.table[0];

            if (query.getFilterExpressions().empty()) {
                // Delete ALL
                DEBUG("query.getFilterExpressions().empty(), delete all")
                size_t count = table->index.size();
                table->index.each([&](const IdentifierHash& id, uint32_t blockNum) {
                    aContext.database->storage.markChainAsFree(blockNum);
                    return true;
                });
                table->index.clear();
                table->cache.clear();
                DEBUG("table->index.clear()")
                DEBUG("table->cache.clear()")
                return {Errors::noError, count};
            }

            DEBUG("!query.getFilterExpressions().empty()")
            std::vector<IdentifierHash> toRemove;
            table->forEachRow(
                    [&](const TableRow& aRow) {
                        IdentifierHash id = aRow.getIdentifier();
                        uint32_t block = *table->index.getBlock(id);
                        aContext.database->storage.markChainAsFree(block);
                        toRemove.push_back(id);
                        return true;
                    },
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
            for (IdentifierHash id : toRemove) {
                table->index.remove(id);
                table->cache.remove(id);
                DEBUG("table->index.remove(id), where id=", id)
                DEBUG("table->cache.remove(id), where id=", id)
            }

            size_t updatedSize = toRemove.size();
            // Kinda cheating by keeping the unbounded cache through multiple queries so clearing it here.
            table->cache.clear();
            DEBUG("table->cache.clear()")
            INFO("DeleteStatement finish running")
            return {Errors::noError, updatedSize};
        }


        DeleteParser& deleteParser;
    };
}

#endif //ECE141DB_DELETESTATEMENT_HPP
