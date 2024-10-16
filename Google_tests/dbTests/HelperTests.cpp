//
// Created by Daniel Ziper on 4/18/24.
//

#include <gtest/gtest.h>
#include "../include/misc/OrderedMap.hpp"

using namespace ECE141;

TEST(OrderedMap, Basic) {
    OrderedMap<std::string, int> map;
    EXPECT_TRUE(map.insert("abc", 123));
    EXPECT_TRUE(map.insert("hello", 5));
    EXPECT_TRUE(map.insert("aaa", 20));
    EXPECT_FALSE(map.insert("hello", 10000));

    std::vector<std::string> expected {"abc", "hello", "aaa"};
    std::vector<int> expectedInts {123, 5, 20};
    size_t idx = 0;
    for (auto iter : map) {
        EXPECT_EQ(iter.first, expected[idx]);
        EXPECT_EQ(iter.second, expectedInts[idx]);
        idx++;
    }
    EXPECT_EQ(idx, 3);
}
