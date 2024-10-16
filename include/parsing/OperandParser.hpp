//
// Created by YING JIEH XIA on 2024/5/7.
//

#ifndef ECE141DB_OPERANDPARSER_HPP
#define ECE141DB_OPERANDPARSER_HPP

#include "misc/Filters.hpp"
#include "Parser.hpp"

namespace ECE141 {
    struct OperandParser : public SingleParser<Operand> {

        bool operator()(TokenParser &tokenParser) override {
            INFO("OperandParser start parsing")
            Tokenizer& tokenizer = tokenParser.tokenizer;
            Token& theCurrent = tokenizer.current();

            if (tokenParser.currentIsType(TokenType::identifier)) {
                TableFieldParser fieldParser;
                if (tokenParser.parseWith(fieldParser)) { out.setField(fieldParser.out); }
                else return false;
            } else if (tokenParser.currentIsType(TokenType::number)) {
                out.setNumber(theCurrent);
                tokenizer.next();
            } else if (tokenParser.currentIsType(TokenType::string)) {
                out.setVarChar(theCurrent);
                tokenizer.next();
            } else if (tokenParser.currentIsType(TokenType::keyword) && Helpers::in_array({Keywords::true_kw, Keywords::false_kw}, theCurrent.keyword)) {
                out.setBool(theCurrent);
                tokenizer.next();
            } else {
                ERROR("unknownType")
                error = Errors::unknownType;
            }
            DEBUG("Parsed ", theCurrent.data)
            INFO("OperandParser finish parsing")
            return error == Errors::noError;
        }
    };
}

#endif //ECE141DB_OPERANDPARSER_HPP
