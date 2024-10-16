//
// Created by Daniel Ziper on 5/25/24.
//



#ifndef ECE141DB_JOINPARSER_HPP
#define ECE141DB_JOINPARSER_HPP

#include "Parser.hpp"
#include "misc/DBQuery.hpp"
#include "misc/Helpers.hpp"

namespace ECE141 {
    struct JoinParser : public Parser {
        JoinParser(DBQuery& aQuery) : theQuery(aQuery) {}

        bool operator()(TokenParser& tokenParser) override {
            INFO("JoinParser start parsing")
            while (tokenParser.checkNextInList(Helpers::joinModifiers)) {
                Keywords kw;
                tokenParser.getKeyword(kw);
                theQuery.joinKeywords.push_back(kw);
                DEBUG("theQuery.joinKeywords.push_back(kw), where kw=", Helpers::keywordToString(kw))
            }
            TableNameParser tnp;
            OpSepExpressionParser osep;
            returnIfError(
                    tokenParser.nextIs({Keywords::join_kw}).parseWith(tnp)
                    .nextIs({Keywords::on_kw}).parseWith(osep))

            theQuery.tableNames.push_back(tnp.out);
            DEBUG("theQuery.tableNames.push_back(tnp.out), where tnp.out.table=", tnp.out.table)
            for (auto& expr : osep.out) {
                if (expr.op != Operators::equal_op) return false;
                theQuery.joinOn.add(expr);
            }
            INFO("JoinParser finish parsing")
            return true;
        }
    protected:
        DBQuery& theQuery;
    };
}

#endif //ECE141DB_JOINPARSER_HPP
