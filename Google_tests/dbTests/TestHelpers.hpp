//
// Created by Daniel Ziper on 4/22/24.
//

#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP


#include <gtest/gtest.h>
#include "../include/storage/Storage.hpp"
#include "../include/parsing/TokenParser.hpp"

using namespace ECE141;

#define EXPECT_SUCC(res) EXPECT_EQ(res.error, Errors::noError)
#define EXPECT_ERR(res, err) EXPECT_EQ(res.error, err)

void helperTestStorable(Storable* in, Storable* out);

class QuickParser : public TokenParser {
public:
    std::stringstream mySS;
    Tokenizer tokenizer;
    QuickParser(const std::string& query) : TokenParser(tokenizer), tokenizer(mySS) {
        mySS << query;
        tokenizer.tokenize();
    }
    QuickParser(std::stringstream& ss) : TokenParser(tokenizer), tokenizer(ss) {
        tokenizer.tokenize();
    }
};

#endif
