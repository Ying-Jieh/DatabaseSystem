//
// Created by Daniel Ziper on 4/21/24.
//

#ifndef ECE141DB_ATTRIBUTEPARSER_HPP
#define ECE141DB_ATTRIBUTEPARSER_HPP

#include "Parser.hpp"
#include "database/Schema.hpp"

namespace ECE141 {
    struct AttributeParser : public SingleParser<Attribute> {

        bool operator()(TokenParser &tokenParser) override {
            INFO("AttributeParser start parsing")
            std::string identifier;
            Keywords typeKW;
            KWList typeKWList{
                Keywords::boolean_kw, Keywords::char_kw, Keywords::datetime_kw, Keywords::integer_kw,
                Keywords::timestamp_kw, Keywords::float_kw, Keywords::varchar_kw
            };

            if (tokenParser.getIdentifier(identifier).getKeywordIn(typeKW, typeKWList)) {
                DEBUG("Parsed Identifier=", identifier)
                out = Attribute{identifier, Helpers::keywordToDataType(typeKW)};
            } else {
                ERROR("identifierExpected")
                error = Errors::identifierExpected;
                return false;
            }

            if (tokenParser.tokenizer.skipIf(left_paren)) {
                int length;
                if (!tokenParser.getNumber(length)) {
                    error = Errors::valueExpected;
                }
                if (!tokenParser.tokenizer.skipIf(right_paren)) {
                    error = Errors::punctuationExpected;
                    return false;
                }
                out.field_length = length;
            }

            while (!tokenParser.currentIsType(TokenType::punctuation)) {
                // All of these checks autoskip, so no infinite loop (I hope)
                if (tokenParser.checkNextIs({Keywords::not_kw, Keywords::null_kw})) {
                    DEBUG("Parsed 'not null'")
                    out.nullable = false;
                } else if (tokenParser.checkNextIs({Keywords::auto_increment_kw})) {
                    DEBUG("Parsed 'auto increment'")
                    out.auto_increment = true;
                } else if (tokenParser.checkNextIs({Keywords::primary_kw, Keywords::key_kw})) {
                    DEBUG("Parsed 'primary key'")
                    out.primary_key = true;
                    out.unique = true; // I think so
                } else if (tokenParser.checkNextIs({Keywords::unique_kw})) {
                    DEBUG("Parsed 'unique'")
                    out.unique = true;
                } else if (tokenParser.checkNextIs({Keywords::default_kw})) {
                    DEBUG("Parsed 'default', default=", tokenParser.tokenizer.current().data)
                    out.default_value = Helpers::stringToValue(tokenParser.tokenizer.current().data, out.type);
                    tokenParser.skip(1);
                    break;
                } else {
                    ERROR("unexpectedKeyword")
                    error = Errors::unexpectedKeyword;
                    break;
                }
            }
            INFO("AttributeParser finish parsing")
            return error == Errors::noError;
        }
    };
}
#endif //ECE141DB_ATTRIBUTEPARSER_HPP
