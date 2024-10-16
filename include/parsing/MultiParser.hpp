//
// Created by Daniel Ziper on 4/26/24.
//

#ifndef ECE141DB_MULTIPARSER_HPP
#define ECE141DB_MULTIPARSER_HPP

#include <vector>
#include "Parser.hpp"
#include "TokenParser.hpp"
#include "ValueParser.hpp"
#include "ExpressionParser.hpp"

namespace ECE141 {

    template<typename T>
    class MultiParser : public SingleParser<std::vector<T>> {
    public:
        explicit MultiParser(SingleParser<T>& aParser, const std::string aSeparator=",",
                    const std::string aStart="(", const std::string aEnd=")") :
                    parser(aParser), separator(aSeparator), startSymbol(aStart), endSymbol(aEnd) {}
        bool operator()(TokenParser &tokenParser) override {
            bool success = true;

            if (!startSymbol.empty())
                tokenParser.skipIfData(startSymbol, success);

            bool hadSeparator = true;
            while (success && hadSeparator) {
                parser.reset();
                bool isSemicolon = false;
                tokenParser.skipIfData(";", isSemicolon, false);
                if (isSemicolon) {
                    hadSeparator = false;
                    break;
                }
                returnIfError(tokenParser.tryParseWith(parser, success));
                this->out.push_back(parser.out);
                if (!separator.empty()) {
                    tokenParser.skipIfData(separator, hadSeparator, false);
                } else {
                    if (!tokenParser.tokenizer.more() || tokenParser.currentIsType(TokenType::keyword)) {
                        hadSeparator = false;
                        break;
                    }
                }
            }

            if (hadSeparator || this->out.empty()) {
               this->error = Errors::parsingError;
                return false;
            }

            if (!endSymbol.empty())
                tokenParser.skipIfData(endSymbol, success);

            return true;
        }

        virtual void reset() override {
            this->out.clear();
            Parser::error = Errors::noError;
        }

    protected:
        SingleParser<T>& parser;
        std::string separator;
        std::string startSymbol;
        std::string endSymbol;
        const bool requireSymbols = true;
    };


    struct CommaSepFieldParser : public MultiParser<TableField> {
        TableFieldParser tfp;
        CommaSepFieldParser() : MultiParser<TableField>(tfp, ",", "(", ")") {}
    };

    struct CommaSepValueParser : public MultiParser<Value> {
        ValueParser vp;
        CommaSepValueParser() : MultiParser<Value>(vp, ",", "(", ")") {}
    };

    struct CommaSepExpressionParser : public MultiParser<Expression> {
        ExpressionParser exParser;
        CommaSepExpressionParser() : MultiParser<Expression>(exParser, ",", "", "") {}
    };

    struct OpSepExpressionParser : public MultiParser<Expression> {
        ExpressionParser exParser;
        OpSepExpressionParser() : MultiParser<Expression>(exParser, "", "", "") {}
    };

}

#endif //ECE141DB_MULTIPARSER_HPP
