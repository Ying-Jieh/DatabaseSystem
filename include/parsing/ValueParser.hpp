//
// Created by YING JIEH XIA on 2024/4/25.
//

#ifndef ECE141DB_VALUEPARSER_HPP
#define ECE141DB_VALUEPARSER_HPP

#include "Parser.hpp"

namespace ECE141 {
    struct ValueParser : public SingleParser<Value> {
        bool operator()(TokenParser& tokenParser) override {
            INFO("ValueParser start parsing")
            Token& current = tokenParser.tokenizer.current();
            if (current.type == TokenType::number) {
                DEBUG("Parsed a number: ", current.data)
                if (current.data.find('.', 0) == std::string::npos) {
                    out = std::stoi(current.data);
                } else {
                    out = std::stod(current.data);
                }
            } else if (current.type == TokenType::keyword) {
                DEBUG("Parsed a keyword: ", current.data)
                if (current.keyword == Keywords::null_kw) {
                    Null null;
                    out = null;
                } else if (current.keyword == Keywords::true_kw) {
                    out = true;
                } else if (current.keyword == Keywords::false_kw) {
                    out = false;
                } else {
                    error = Errors::valueExpected;
                }
            } else if (current.type == TokenType::string) {
                DEBUG("Parsed a string: ", current.data)
                out = current.data;
            } else {
                ERROR("unknownType")
                error = Errors::unknownType;
            }
            tokenParser.tokenizer.next();
            INFO("valueParser finish parsing")
            return error == Errors::noError;
        }
    };

    struct TableFieldParser : public SingleParser<TableField> {
    public:
        bool operator()(TokenParser& tokenParser) override {
            INFO("TableFieldParser start parsing")
            std::string loggingStr{};
            Tokenizer& theTokenizer = tokenParser.tokenizer;
            Token &theToken = theTokenizer.current();
            if (TokenType::identifier == theToken.type) {
                out.fieldName = theToken.data;
                loggingStr = out.fieldName;
                theTokenizer.next();
                if(theTokenizer.skipIf('.')) {
                    theToken = theTokenizer.current();
                    if (TokenType::identifier == theToken.type) {
                        theTokenizer.next(); //yank it...
                        out.table = out.fieldName;
                        out.fieldName = theToken.data;
                        out.alias = out.table + "." + out.fieldName;
                        loggingStr = out.alias;
                    } else {
                        ERROR("identifierExpected, expected a filedName after '.'")
                        error = Errors::identifierExpected;
                    }
                }
            } else {
                ERROR("identifierExpected")
                error = Errors::identifierExpected;
            }
            DEBUG("Parsed ", loggingStr)

            if (out.alias.empty()) out.alias = out.fieldName;
            INFO("TableFieldParser finish parsing")
            return error == Errors::noError;
        };
    };

    struct TableNameParser : public SingleParser<TableName> {
    public:
        bool operator()(TokenParser& tokenParser) override {
            INFO("TableNameParser start parsing")
            Tokenizer& tokenizer = tokenParser.tokenizer;
            Token& theToken = tokenizer.current();
            if (TokenType::identifier == theToken.type) {
                out.table = theToken.data;
                DEBUG("Parsed a tableName: ", out.table)
                error = Errors::noError;
                tokenizer.next(); //skip ahead...
                if(tokenizer.skipIf(Keywords::as_kw)) { //try to skip 'as' for alias...
                    theToken=tokenizer.current();
                    out.alias=theToken.data; //copy alias...
                    DEBUG("Parsed an alias: ", out.alias)
                    tokenizer.next(); //skip past alias...
                }
            } else {
                ERROR("identifierExpected, expected a tableName")
                error = Errors::identifierExpected;
            }

            INFO("TableNameParser finish parsing")
            return error == Errors::noError;
        };
    };
}






#endif //ECE141DB_VALUEPARSER_HPP
