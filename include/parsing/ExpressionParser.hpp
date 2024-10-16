//
// Created by YING JIEH XIA on 2024/5/6.
//

#ifndef ECE141DB_EXPRESSIONPARSER_HPP
#define ECE141DB_EXPRESSIONPARSER_HPP

#include "Parser.hpp"
#include "OperandParser.hpp"
#include "OperatorParser.hpp"

namespace ECE141 {
    struct ExpressionParser : public SingleParser<Expression> {
    public:
        bool hasNOT = false;
        bool operator()(TokenParser& tokenParser) override {
            INFO("ExpressionParser start parsing")
            if (tokenParser.checkNextIs({Keywords::not_kw})) { hasNOT = true; }
            OperandParser lhsOperandParser;
            OperandParser rhsOperandParser;
            OperatorParser operatorParser;
            if (tokenParser.parseWith(lhsOperandParser).parseWith(operatorParser).parseWith(rhsOperandParser)) {
                out.lhs = lhsOperandParser.out;
                out.op = hasNOT ? Helpers::inverseOp(operatorParser.out) : operatorParser.out;
                out.rhs = rhsOperandParser.out;
            } else {
                ERROR("syntaxError")
                error = Errors::syntaxError;
            }

            Keywords theLogicKW = tokenParser.tokenizer.current().keyword;
            if (Helpers::in_array({Keywords::not_kw, Keywords::or_kw, Keywords::and_kw}, theLogicKW)) {
                out.logic = Helpers::KeywordToLogical(theLogicKW);
                tokenParser.skip(1);
            }
            INFO("ExpressionParser finish parsing")
            return error == Errors::noError;
        }
    };

}


#endif //ECE141DB_EXPRESSIONPARSER_HPP
