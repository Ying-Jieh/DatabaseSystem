//
// Created by Daniel Ziper on 5/12/24.
//

#ifndef ECE141DB_UPDATESTATEMENT_HPP
#define ECE141DB_UPDATESTATEMENT_HPP

#include "statement/Statement.hpp"
#include "misc/DBQuery.hpp"
#include "database/Table.hpp"
#include "parsing/UpdateParser.hpp"

namespace ECE141 {
    class UpdateStatement : public Statement {
    public:
        UpdateStatement(UpdateParser& anUpdateParser) : updateParser(anUpdateParser) {}

        StatusResult parse(TokenParser &tokenParser) override {
            return tokenParser.clear().parseWith(updateParser).getState();
        }

        StatusResult run(ExecutionContext &aContext) override {
            INFO("UpdateStatement start running")
            DBQuery& query = updateParser.theQuery;
            returnIfError(query.resolveTablesAndFields(aContext));
            Table* table = query.table[0];

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

            StatusResult error;
            table->cache.eachRequired([&](const IdentifierHash& aHash, const TableRow& aRow, const CacheStatus& _){
                bool dirty = false;
                auto updated = updateWithExpressions(aRow, query.setExpressions, dirty);
                if (!updated.has_value()) {
                    ERROR("syntaxError")
                    error = {Errors::syntaxError};
                    return false;
                }
                error = table->validate(aRow.getValues());
                if (!error) return false;
                if (dirty) {
                    // TODO: Check for Primary Key change! Requires updating cache and index IdentifierHash.
                    table->cache.updateRow(aHash, *updated, false, true);
                }
                return true;
            });

            bool success = table->cache.eachDirty([&](const IdentifierHash& id, const TableRow& row, const CacheStatus&){
                if (auto idx = table->index.getBlock(id)){
                    aContext.database->storage.writeStorable(row, *idx);
                    return true;
                }
                return false;
            });
            if (!success) {
                ERROR("indexError")
                return {Errors::indexError};
            }
            table->cache.resetDirty();

            size_t updatedSize = table->cache.requiredSize();
            // Kinda cheating by keeping the unbounded cache through multiple queries so clearing it here.
            table->cache.clear();

            INFO("UpdateStatement finish running")
            return {Errors::noError, updatedSize};
        }

        std::optional<TableRow> updateWithExpressions(const TableRow& aRow, const Expressions& aUpdateExpressions, bool& dirty) {
            TableRow updated = aRow;
            for (auto expr : aUpdateExpressions) {
                // Note: copy expression before evaluating
                expr.rhs.evaluate(aRow);
                auto res = updated.updateKey(expr.lhs.tableField.fieldName, expr.rhs.value);
                if (res.value()) {
                    dirty = dirty | res.value();
                } else {
                    return std::nullopt;
                }
            }
            return updated;
        }

        UpdateParser& updateParser;
    };
}

#endif //ECE141DB_UPDATESTATEMENT_HPP
