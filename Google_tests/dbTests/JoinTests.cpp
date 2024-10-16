//
// Created by Daniel Ziper on 5/24/24.
//

#include "gtest/gtest.h"
#include "../include/misc/joins/Join.hpp"

using namespace ECE141;

TEST(TestJoinFactory, Basic) {
    Table table;
    DBQuery query;
    query.table.push_back(&table);
    auto ptr = joinFactory(query);
//    JoinedRow row{{}, {}};
}
