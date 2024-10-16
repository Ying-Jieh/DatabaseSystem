//
// Created by Daniel Ziper on 4/22/24.
//

#include <gtest/gtest.h>
#include "../include/database/Database.hpp"

using namespace ECE141;

TEST(CreateTable, Basic) {
    Table table("users");
    table.addAttribute({"id", DataTypes::int_type});
    table.addAttribute({"name", DataTypes::varchar_type});

}

TEST(BuildRow, Basic) {
    Table table("users");

    Attribute id{"id", ECE141::DataTypes::int_type};
    id.auto_increment = true;
    id.primary_key = true;
    id.unique = true;

    table.addAttribute(id);

    Attribute name{"name", ECE141::DataTypes::varchar_type};
    table.addAttribute(name);

    StatusResult res;
    TableRow out = table.buildRow({"Bob"}, {{"name"}}, res);
    EXPECT_EQ(res.error, Errors::noError);
    EXPECT_EQ(out.size(), 2);
    EXPECT_EQ(out[0], Value(AutoIncrementer()));
    EXPECT_EQ(out[1], Value("Bob"));

    table.addRow(out);
    table.addRow(out);

    EXPECT_EQ(table.cache.getRow(1)[0], Value(1));
    EXPECT_EQ(table.cache.getRow(2)[0], Value(2));
}

TEST(TableRow, storable) {
    Table table("users");
//    Attribute id{"id", ECE141::DataTypes::int_type};
//    id.auto_increment = true;
//    id.primary_key = true;
//    id.unique = true;

    Attribute name{"name", ECE141::DataTypes::varchar_type};
    name.field_length = 20;
    name.default_value = "anonymous";

    Attribute age{"age", ECE141::DataTypes::int_type};
    age.default_value = 99;

//    table.addAttribute(id);
    table.addAttribute(name);
    table.addAttribute(age);

    StatusResult res;
    TableRow r1 = table.buildRow({"Bob", 18}, {{"name"}, {"age"}}, res);
    EXPECT_EQ(res.error, Errors::noError);
    table.addRow(r1);

    std::ostringstream oss(std::ios::binary);
    r1.encode(oss);

    std::istringstream iss(oss.str());
    TableRow r2{&table, {}};
    table.addRow(r2);
    EXPECT_EQ(r2.decode(iss).error, Errors::noError);
    EXPECT_EQ(r1.getValues(), r2.getValues());
}


