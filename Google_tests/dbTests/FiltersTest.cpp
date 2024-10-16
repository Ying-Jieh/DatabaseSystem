//
// Created by Daniel Ziper on 5/8/24.
//


#include "gtest/gtest.h"
#include "database/Schema.hpp"
#include "database/Table.hpp"
#include "tokenizer/Tokenizer.hpp"
#include "parsing/TokenParser.hpp"
#include "misc/DBQuery.hpp"
#include "parsing/ClauseParser.hpp"
#include "TestHelpers.hpp"

using namespace ECE141;

class FiltersTests : public testing::Test {
protected:
   void SetUp() override {
       Attribute id{"id", ECE141::DataTypes::int_type};
       id.auto_increment = true;
       id.primary_key = true;
       id.unique = true;
       table.addAttributes({
                                   id,
                                   {"name", DataTypes::varchar_type},
                                   {"age", DataTypes::int_type},
                                   {"height", DataTypes::float_type},
                                   {"married", DataTypes::bool_type},
                                   {"country", DataTypes::varchar_type}
                           });

       StatusResult res;
       row1 = table.buildRow({"Bob", 18, 175.7, false, "USA"},   {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       row2 = table.buildRow({"Chris", 24, 179.8, false, "Taiwan"}, {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       row3 = table.buildRow({"Eric", 27, 182.1, false, "Taiwan"},  {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       row4 = table.buildRow({"Lisa", 28, 175.1, true, "China"},   {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       row5 = table.buildRow({"Ben", 31, 184.2, true, "Japan"},    {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       row6 = table.buildRow({"Daniel", 23, 178.6, false, "USA"},    {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       row7 = table.buildRow({"Bobby", 31, 150.1, true, "Japan"},    {{"name"}, {"age"}, {"height"}, {"married"}, {"country"}}, res);
       EXPECT_EQ(res.error, Errors::noError);

       table.addRows({row1, row2, row3, row4, row5, row6, row7});
   }
   Table table{"Users"};
   TableRow row1;
   TableRow row2;
   TableRow row3;
   TableRow row4;
   TableRow row5;
   TableRow row6;
   TableRow row7;
};

TEST_F(FiltersTests, basicAND) {
    std::stringstream ss;
    ss << "WHERE age > 20 AND height > 180";
    QuickParser tokenParser{ss};
    DBQuery dbQuery;
    ClauseParser clauseParser{dbQuery};
    tokenParser.parseWith(clauseParser);

    Filters filter;
    for (auto& expr : dbQuery.getFilterExpressions()) {
        filter.add(expr);
    }

    EXPECT_FALSE(filter.matches(row1));
    EXPECT_FALSE(filter.matches(row2));
    EXPECT_TRUE(filter.matches(row3));
    EXPECT_FALSE(filter.matches(row4));
    EXPECT_TRUE(filter.matches(row5));
    EXPECT_FALSE(filter.matches(row6));
}

TEST_F(FiltersTests, basicOR) {
    std::stringstream ss;
    ss << "WHERE age > 30 OR height > 180";
    QuickParser tokenParser{ss};
    DBQuery dbQuery;
    ClauseParser clauseParser{dbQuery};
    tokenParser.parseWith(clauseParser);

    Filters filter;
    for (auto& expr : dbQuery.getFilterExpressions()) {
        filter.add(expr);
    }

    EXPECT_FALSE(filter.matches(row1));
    EXPECT_FALSE(filter.matches(row2));
    EXPECT_TRUE(filter.matches(row3));
    EXPECT_FALSE(filter.matches(row4));
    EXPECT_TRUE(filter.matches(row5));
    EXPECT_FALSE(filter.matches(row6));
    EXPECT_TRUE(filter.matches(row7));
}

TEST_F(FiltersTests, basicNOT) {
    std::stringstream ss;
    ss << "WHERE NOT country = 'Taiwan'";
    QuickParser tokenParser{ss};
    DBQuery dbQuery;
    ClauseParser clauseParser{dbQuery};
    tokenParser.parseWith(clauseParser);

    Filters filter;
    for (auto& expr : dbQuery.getFilterExpressions()) {
        filter.add(expr);
    }

    EXPECT_TRUE(filter.matches(row1));
    EXPECT_FALSE(filter.matches(row2));
    EXPECT_FALSE(filter.matches(row3));
    EXPECT_TRUE(filter.matches(row4));
    EXPECT_TRUE(filter.matches(row5));
    EXPECT_TRUE(filter.matches(row6));
}


