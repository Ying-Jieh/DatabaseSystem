//
// Created by YING JIEH XIA on 2024/5/15.
//

#ifndef ECE141DB_DELETEPARSER_HPP
#define ECE141DB_DELETEPARSER_HPP

#include "Parser.hpp"
#include "misc/DBQuery.hpp"
#include "ValueParser.hpp"
#include "MultiParser.hpp"

namespace ECE141 {
    struct DeleteParser : public Parser {
        explicit DeleteParser(DBQuery& aQuery) : theQuery(aQuery) {}

        bool operator()(TokenParser& tokenParser) override {
            INFO("DeleteParser start parsing")
            TableNameParser tableNameParser;
            OpSepExpressionParser oseParser;
            returnIfError(
                    tokenParser.nextIs({Keywords::delete_kw, Keywords::from_kw})
                    .parseWith(tableNameParser)
                    .nextIs({Keywords::where_kw})
                    .parseWith(oseParser)
            )

            theQuery.tableNames.push_back(tableNameParser.out);
            TRACE("theQuery.tableNames.push_back(tableName), where tableName=", tableNameParser.out.table)
            theQuery.addExpressions(oseParser.out);
            TRACE("theQuery.addExpressions(oseParser.out)")
            INFO("DeleteParser finish parsing")
            return true;
        }

        DBQuery& theQuery;
    };
}



#endif //ECE141DB_DELETEPARSER_HPP
