//
// Created by Daniel Ziper on 4/21/24.
//

#ifndef ECE141DB_CREATETABLEPARSER_HPP
#define ECE141DB_CREATETABLEPARSER_HPP

#include "Parser.hpp"
#include "AttributeParser.hpp"
#include "database/Table.hpp"

namespace ECE141 {
    struct CreateTableParser : Parser {
        Table table;

        CreateTableParser(std::shared_ptr<Database> db = nullptr) : table("", db) {};

        bool operator()(TokenParser &tokenParser) override {
            INFO("CreateTableParser start parsing")
            std::string tableName;
            if (!tokenParser.nextIs({Keywords::create_kw, Keywords::table_kw}).getIdentifier(tableName)) {
                ERROR("identifierExpected")
                error = {Errors::identifierExpected};
                return false;
            }
            if (!tokenParser.tokenizer.skipIf(left_paren)) {
                ERROR("punctuationExpected (left_paren)")
                error = {Errors::punctuationExpected};
                return false;
            }
            table.name = tableName;

            AttributeParser attributeParser;
            std::vector<Attribute> attributes;
            bool trailingComma = true;
            while (!tokenParser.tokenizer.skipIf(right_paren)) {
                if (!trailingComma) {
                    ERROR("punctuationExpected (comma)")
                    error = Errors::punctuationExpected;
                    return false;
                }
                if (tokenParser.parseWith(attributeParser)) {
                    table.addAttribute(attributeParser.out);
                    trailingComma = tokenParser.tokenizer.skipIf(comma);
                } else {
                    return false;
                }
            }
            INFO("CreateTableParser finish parsing")
            return true;
        }
    };
}

#endif //ECE141DB_CREATETABLEPARSER_HPP
