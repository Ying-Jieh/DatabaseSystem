//
// Created by YING JIEH XIA on 2024/4/22.
//

#ifndef ECE141DB_STATEMENT_HPP
#define ECE141DB_STATEMENT_HPP


#include "parsing/Parser.hpp"
#include "ExecutionContext.hpp"

namespace ECE141 {
    class Statement {
    public:
        virtual StatusResult parse(TokenParser& tokenParser) = 0;
        virtual StatusResult run(ExecutionContext& aContext) = 0;
    };
}



#endif //ECE141DB_STATEMENT_HPP
