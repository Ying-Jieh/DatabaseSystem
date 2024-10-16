//
// Created by YING JIEH XIA on 2024/5/5.
//

#ifndef ECE141DB_SELECTPARSER_HPP
#define ECE141DB_SELECTPARSER_HPP

#include <vector>
#include "Parser.hpp"
#include "misc/DBQuery.hpp"
#include "MultiParser.hpp"
#include "ExpressionParser.hpp"
#include "ClauseParser.hpp"

namespace ECE141 {
    class SelectParser : public Parser {
    public:
        DBQuery theQuery;

        bool operator()(TokenParser& tokenParser) override {
            INFO("SelectParser start parsing")
            if (!tokenParser.nextIs({Keywords::select_kw})) {
                ERROR("syntaxError, expected 'select'")
                error = Errors::syntaxError;
                return false;
            }

            // ------- Parse Select -------
            Token& currentToken = tokenParser.tokenizer.current();
            if (currentToken.type == TokenType::operators) {
                if (!tokenParser.tokenizer.skipIf('*')) {
                    ERROR("syntaxError, expected '*'")
                    error = Errors::syntaxError;
                    return false;
                }
            } else if (currentToken.type == TokenType::identifier) {
                TableFieldParser tfp;
                MultiParser mp{tfp, ",", "", ""};
                returnIfError(tokenParser.parseWith(mp));
                theQuery.selectedFields = mp.out;
                theQuery.setAll(false);
            } else {
                ERROR("syntaxError")
                error = Errors::syntaxError;
                return false;
            }

            if (!tokenParser.nextIs({Keywords::from_kw})) {
                ERROR("syntaxError, expected 'from'")
                error = Errors::syntaxError;
                return false;
            }

            TableNameParser tableNameParser;
            returnIfError(tokenParser.parseWith(tableNameParser));
            theQuery.tableNames.push_back(tableNameParser.out);
            DEBUG("theQuery.tableNames.push_back(tableName), where tableName=", tableNameParser.out.table)

            // ------- Parse Clauses -------
            std::set<Keywords> seenClauses;
            ClauseParser clauseParser(theQuery);
            while (tokenParser.tokenizer.more() && tokenParser.getState().error == Errors::noError) {
                if (tokenParser.currentIsType(TokenType::keyword)) {
                    currentToken = tokenParser.tokenizer.current();
                    if (seenClauses.find(currentToken.keyword) == seenClauses.end()) {
                        returnIfError(tokenParser.parseWith(clauseParser))
                        seenClauses.insert(currentToken.keyword);
                    }
                    else {
                        ERROR("invalidArguments")
                        error = Errors::invalidArguments;
                        return false;
                    }
                }
                else if (tokenParser.skip(';')) break;
                else {
                    ERROR("keywordExpected")
                    error = Errors::keywordExpected;
                    return false;
                }
            }
            INFO("SelectParser finish parsing")
            return true;
        }
    };
}



#endif //ECE141DB_SELECTPARSER_HPP
