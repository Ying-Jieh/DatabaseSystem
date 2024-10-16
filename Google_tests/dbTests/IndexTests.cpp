//
// Created by Daniel Ziper on 4/22/24.
//

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "../include/database/Index.hpp"
#include "TestHelpers.hpp"

using namespace ECE141;

TEST(IndexTests, Basic) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int min = 1;
    int max = 1000000;
    size_t count = 100;
    std::uniform_int_distribution<> dis(min, max);

    Index index;
    for (size_t i = 0; i < count; i++) {
        ASSERT_TRUE(index.add(dis(gen), dis(gen))) << "This test may randomly fail because of generating the same random number twice, just rerun it";
    }
    Index index2;
    helperTestStorable(&index, &index2);

    EXPECT_EQ(index, index2);
    EXPECT_EQ(index2.size(), count);
    EXPECT_EQ(index.size(), count);
}

TEST(IndexTests, StringTemplate) {
    std::vector<std::string> strings {"Bob", "Hello", "Hello World", "hi124p9710387r!@#@00a"};
    Index<std::string> index;
    size_t count = 0;
    for (auto& str : strings) {
        index.add(str, count);
        count++;
    }

    EXPECT_EQ(index.size(), strings.size());

    Index<std::string> index2;
    helperTestStorable(&index, &index2);

    EXPECT_EQ(index, index2);
    EXPECT_EQ(index2.size(), strings.size());
}

TEST(IndexTests, StringIndex) {
    std::vector<std::string> strings {"Bob", "Hello", "Hello World", "hi124p9710387r!@#@00a"};
    StringIndex index;
    size_t count = 0;
    for (auto& str : strings) {
        index.add(str, count);
        count++;
    }

    EXPECT_EQ(index.size(), strings.size());

    StringIndex index2;
    helperTestStorable(&index, &index2);

    EXPECT_EQ(index, index2);
    EXPECT_EQ(index2.size(), strings.size());

    std::hash<std::string> hasher;
    for (auto& str : strings) {
        EXPECT_EQ(index2.lookupHash(hasher(str)), str);
    }
}
