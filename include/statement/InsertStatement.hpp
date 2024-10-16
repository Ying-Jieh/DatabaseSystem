//
// Created by Daniel Ziper on 4/25/24.
//

#ifndef ECE141DB_INSERTSTATEMENT_HPP
#define ECE141DB_INSERTSTATEMENT_HPP

#include "database/Database.hpp"
#include "parsing/InsertParser.hpp"
#include "Statement.hpp"

#include <utility>

namespace ECE141 {
    class InsertStatement : public Statement {
    public:
        InsertStatement(InsertParser& aParser) : parser(aParser) {}
        StatusResult parse(TokenParser &tokenParser) override {
            return tokenParser.clear().parseWith(parser).getState();
        }

        StatusResult run(ExecutionContext& aContext) override {
            INFO("InsertStatement start running")
            Table* table = aContext.database->getTable(parser.tableName);
            if (!table) {
                ERROR("unknownTable")
                return {Errors::unknownTable};
            }

            for (const auto& values : parser.values) {
                if (values.size() != parser.fields.size() ||
                    values.size() > table->attributes.size()) {
                    ERROR("keyValueMismatch")
                    return {Errors::keyValueMismatch};
                }
            }

            StatusResult theResult;

            std::vector<TableRow> toAdd;
            for (const auto& values : parser.values) {
                TableRow row = table->buildRow(values, parser.fields, theResult);
                if (!theResult) {
                    return {theResult};
                }
                toAdd.push_back(row);
            }

            for (const auto& row : toAdd) {
                // TODO: eventually change to addRows(), which should add all rows together and can revert if failed constraints
                returnIfError(table->addRow(row, false)); // pre-validated by buildRow
            }

            theResult.value = toAdd.size();
            INFO("InsertStatement finish running")
            return theResult;
        }

        InsertParser& parser;
    };
}

#endif //ECE141DB_INSERTSTATEMENT_HPP
