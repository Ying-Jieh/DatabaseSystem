//
// Created by Daniel Ziper on 5/8/24.
//

#include "gtest/gtest.h"
#define protected public
#include "../include/misc/Filters.hpp"
#undef protected
#include "../include/database/Database.hpp"
#include "../include/parsing/TokenParser.hpp"
#include "../include/parsing/CreateTableParser.hpp"
#include "../include/database/RowSelection.hpp"
#include "../include/view/TabularView.hpp"
#include "../include/misc/joins/Join.hpp"

using namespace ECE141;

class SelectTests : public testing::Test {
protected:
    void SetUp() override {
        CreateTableParser creater{};
        std::stringstream ss;
        ss << "create table users (id int primary key, name varchar, weight float)";
        Tokenizer tok(ss);
        tok.tokenize();
        TokenParser parser(tok);
        EXPECT_TRUE(parser.parseWith(creater));
        table = creater.table;

        for (const auto& v : values) {
            table.addRow({&table, v});
        }

        table.each([&](const Row& row){
            auto iter = std::find(values.begin(), values.end(), row.getValues());
            EXPECT_NE(iter, values.end());
            return true;
        });
    };
    std::string dbname = "integrationTest";
    std::shared_ptr<Database> loaded;
    std::vector<std::vector<Value>> values {
            {5, "bob", 123.3},
            {3, "billy", 645.3},
            {20, "billy", 645.0},
            {1, "dave", 645.0},
            {10, "joe", 100.5},
            {21, "joe", 200.5},
            {19, "daniel", 234.0}
    };
    Table table;
};

TEST_F(SelectTests, SimpleRowSelec) {
    // Select weight, name AS first_name from users
    std::vector<TableField> outputFields = {{"weight", "users"}, {"name", "users", "first_name"}};
    RowSelection selection{outputFields, {&table}};
    NoJoin noJoin {outputFields, &table};
    noJoin.each([&](const Row& aRow) {
        selection.add(const_cast<Row *>(&aRow));
        return true;
    });
    EXPECT_EQ(selection.size(), values.size());

    TabularView view{&selection.outputSchema, &selection};
    view.show(std::cout);

    selection.limit({4, 1});
    TabularView view2{&selection.outputSchema, &selection};
    view2.show(std::cout);
    EXPECT_EQ(selection.size(), 1);
}

TEST_F(SelectTests, OrderBy) {
    std::vector<TableField> outputFields = {{"weight", "users"}, {"name", "users", "first_name"}};
    RowSelection selection{outputFields, {&table}};
    NoJoin noJoin {outputFields, &table};
    noJoin.each([&](const Row& aRow) {
        selection.add(const_cast<Row *>(&aRow));
        return true;
    });

    selection.orderBy({{{"name", "users"}, true}});
    TabularView view{&selection.outputSchema, &selection};
    view.show(std::cout);
}


bool testComparator(Filters::ComparableValue lhs, Filters::ComparableValue rhs, Filters::Comparitor comp) {
    return comp(lhs, rhs);
}

TEST(Filters, Basic) {
    // TODO: Double check these
    EXPECT_TRUE(testComparator({4}, {"4"}, Filters::equals));
    EXPECT_TRUE(testComparator({4}, {5}, Filters::lessThan));
    EXPECT_TRUE(testComparator({4}, {"4"}, Filters::lessEqual));
    EXPECT_FALSE(testComparator({"123"}, {"4"}, Filters::greaterThan));
    EXPECT_TRUE(testComparator({"123"}, {"4"}, Filters::lessThan));

    EXPECT_FALSE(testComparator({92120}, {92122}, Filters::greaterThan));
}

TEST_F(SelectTests, Expression) {
    TableField f1 = {"weight", "users"};
    Value val = Null();
    Operand op1{ECE141::TokenType::identifier, f1};
    Operand op2{ECE141::TokenType::number, {123.5}};

    Expression expr {op1, ECE141::Operators::lt_op, op2};
    TableRow row = table.cache.getRow(3);
    EXPECT_FALSE(expr(row));

    expr = {op1, ECE141::Operators::gt_op, op2};
    EXPECT_TRUE(expr(row));
}
