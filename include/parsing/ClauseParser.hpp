//
// Created by YING JIEH XIA on 2024/5/6.
//

#ifndef ECE141DB_CLAUSEPARSER_HPP
#define ECE141DB_CLAUSEPARSER_HPP

#include <set>
#include "Parser.hpp"
#include "ValueParser.hpp"
#include "MultiParser.hpp"
#include "misc/DBQuery.hpp"
#include "JoinParser.hpp"

namespace ECE141 {
    class OrderByFieldParser : public SingleParser<OrderByField> {
        bool operator()(TokenParser& tokenParser) override {
            TableFieldParser fp;
            returnIfError(tokenParser.parseWith(fp));
            out.field = fp.out;
            if (tokenParser.checkNextIs({Keywords::desc_kw})) {
                out.ascending = false;
            } else if (tokenParser.checkNextIs({Keywords::asc_kw})) {
                out.ascending = true;
            } else {
                out.ascending = true;
            }
            return tokenParser.getState();
        }
    };

    class ClauseParser : public Parser {

    public:
        ClauseParser(DBQuery& aQuery) : theQuery(aQuery) {}

        bool operator()(TokenParser& tokenParser) override {
            INFO("ClauseParser start parsing")
            TableFieldParser fp;
            MultiParser mp{fp, ",", "", ""};
            mp.reset();
            if (tokenParser.checkNextIs({Keywords::limit_kw})) {
                DEBUG("Next is limit")
                int start{0};
                int count{0};
                tokenParser.getNumber(start);
                theQuery.limits.push_back(start);
                if (tokenParser.tokenizer.skipIf(',')) {
                    tokenParser.getNumber(count);
                    theQuery.limits.push_back(count);
                }

            } else if (tokenParser.checkNextIs({Keywords::order_kw, Keywords::by_kw})) {
                DEBUG("Next is order by")
                OrderByFieldParser obfp;
                MultiParser omp {obfp, ",", "", ""};
                returnIfError(tokenParser.parseWith(omp));
                theQuery.orderByFields = omp.out;

            } else if (tokenParser.checkNextIs({Keywords::group_kw, Keywords::by_kw})) {
                DEBUG("Next is group by")
                returnIfError(tokenParser.parseWith(mp));
                if (!tokenParser.getState()) {
                    error = tokenParser.getState().error;
                    return false;
                }
                theQuery.groupByFields = mp.out;

            } else if (tokenParser.checkNextIs({Keywords::where_kw})) {
                DEBUG("Next is where")
                OpSepExpressionParser oseParser;
                tokenParser.parseWith(oseParser);
                for (auto& exp : oseParser.out) {
                    theQuery.addExpression(exp);
                }
                if (theQuery.getLastFilterExpression().logic != Logical::no_op) {
                    error = Errors::syntaxError;
                    return false;
                }

            } else if (tokenParser.checkNextInList(
                    Helpers::joinModifiers)) {
                DEBUG("Next is join")
                JoinParser jp{theQuery};
                returnIfError(tokenParser.parseWith(jp))
            } else {
                ERROR("unexpectedKeyword")
                error = Errors::unexpectedKeyword;
                return false;
            }

            return true;
        }

    private:
        DBQuery& theQuery;
    };
}

#endif //ECE141DB_CLAUSEPARSER_HPP
