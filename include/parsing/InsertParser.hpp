//
// Created by Daniel Ziper on 4/25/24.
//

#ifndef ECE141DB_INSERTPARSER_HPP
#define ECE141DB_INSERTPARSER_HPP

#include <vector>
#include "misc/BasicTypes.hpp"
#include "Parser.hpp"
#include "MultiParser.hpp"
#include "ValueParser.hpp"

namespace ECE141 {
    class InsertParser : public Parser {
    public:
        std::string tableName;
        TableFieldList fields;
        std::vector<std::vector<Value>> values;

        bool operator()(TokenParser &tokenParser) override {
            INFO("InsertParser start parsing")
            if (!tokenParser.nextIs({Keywords::insert_kw, Keywords::into_kw}).getIdentifier(tableName)) {
                ERROR("identifierExpected")
                error = Errors::identifierExpected;
                return false;
            }

            CommaSepFieldParser csfP;
            tokenParser.parseWith(csfP);
            fields = csfP.out;

            if (!tokenParser.nextIs({Keywords::values_kw})) {
                ERROR("keywordExpected (value)")
                error = Errors::keywordExpected;
                return false;
            }

            CommaSepValueParser csvP;
            MultiParser<std::vector<Value>> mp{csvP, ",", "", ""};
            tokenParser.parseWith(mp);
            values = mp.out;
            INFO("InsertParser start parsing")
            return true;
        }
    };
}

#endif //ECE141DB_INSERTPARSER_HPP
