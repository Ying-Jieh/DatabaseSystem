//
// Created by YING JIEH XIA on 2024/5/13.
//

#ifndef ECE141DB_UPDATEPARSER_HPP
#define ECE141DB_UPDATEPARSER_HPP

#include "Parser.hpp"
#include "ValueParser.hpp"
#include "ExpressionParser.hpp"
#include "MultiParser.hpp"

namespace ECE141 {
    struct UpdateParser : public Parser {
        explicit UpdateParser(DBQuery& aQuery) : theQuery(aQuery) {}

        bool operator()(TokenParser& tokenParser) override {
            INFO("UpdateParse start parsing")
            TableNameParser tableNameParser;
            returnIfError(tokenParser.nextIs({Keywords::update_kw}).parseWith(tableNameParser))
            theQuery.tableNames.push_back(tableNameParser.out);
            DEBUG("theQuery.tableNames.push_back(tableName), where tableName=", tableNameParser.out.table)

            if (!tokenParser.nextIs({Keywords::set_kw})) {
                ERROR("keywordExpected, expected 'set'")
                error = Errors::keywordExpected;
                return false;
            }

            CommaSepExpressionParser cseParser;
            returnIfError(tokenParser.parseWith(cseParser));
            theQuery.setExpressions = cseParser.out;
            DEBUG("theQuery.setExpressions = cseParser.out;")
            const std::vector<DataTypes> gDTs = {
                    DataTypes::varchar_type, DataTypes::int_type, DataTypes::float_type,
                    DataTypes::bool_type, DataTypes::datetime_type
            };
            for (auto& exp : theQuery.setExpressions) {
                if (exp.lhs.ttype != TokenType::identifier || exp.op != Operators::equal_op || !Helpers::in_array(gDTs, exp.rhs.dtype)) {
                    ERROR("syntaxError")
                    error = Errors::syntaxError;
                    return false;
                }
            }

            if (tokenParser.tokenizer.more() && tokenParser.checkNextIs({Keywords::where_kw})) {
                OpSepExpressionParser oseParser;
                returnIfError(tokenParser.parseWith(oseParser));
                theQuery.addExpressions(oseParser.out);
            }

            return true;
        }

        DBQuery& theQuery;
    };
}

#endif //ECE141DB_UPDATEPARSER_HPP
