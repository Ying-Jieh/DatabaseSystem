//
// Created by YING JIEH XIA on 2024/5/9.
//

#ifndef ECE141DB_OPERATORPARSER_HPP
#define ECE141DB_OPERATORPARSER_HPP

#include "tokenizer/keywords.hpp"
#include "Parser.hpp"

namespace ECE141 {
    struct OperatorParser : SingleParser<Operators> {
        bool operator()(TokenParser& tokenParser) override {
            INFO("OperatorParser start parsing")
            const std::vector<Operators> gOps{
                    Operators::equal_op, Operators::notequal_op,
                    Operators::lt_op, Operators::lte_op,
                    Operators::gt_op, Operators::gte_op
            };

            Token &theToken=tokenParser.tokenizer.current();
            theToken.op = gOperators[theToken.data];
            if (Helpers::in_array<Operators>(gOps, theToken.op)) {
                DEBUG("Parsed ", theToken.data)
                out=theToken.op;
                tokenParser.skip(1);
            } else {
                ERROR("operatorExpected")
                error = Errors::operatorExpected;
            }
            INFO("OperatorParser finish parsing")
            return error==Errors::noError;
        }
    };
}


#endif //ECE141DB_OPERATORPARSER_HPP
