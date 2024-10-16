//
// Created by Daniel Ziper on 4/15/24.
//

#include "gtest/gtest.h"
#include "../Google_tests/dbTests/TestHelpers.hpp"
#include "../include/database/Database.hpp"
#include "../include/view/TabularView.hpp"

using namespace ECE141;

class TableTests : public testing::Test {
protected:
    void SetUp() override {
        t.addAttributes({
                                {"name", DataTypes::varchar_type},
                                {"age", DataTypes::int_type},
                                {"weight", DataTypes::float_type},
                                {"friend", DataTypes::varchar_type},
                                {"married", DataTypes::bool_type}
        });

        r = TableRow(&t, {"Bob", 25, 200.5, Null(), false});
        ids.push_back(r.getIdentifier());
        t.addRow(r);
        r.updateKey("name", "Charlie Chaplin");
        r.updateKey("age", 5);
        ids.push_back(r.getIdentifier());

        t.addRow(r);
        TableRow r2 = {&t, {"Billy", 12312312, 23.8, "Bob", true}};
        ids.push_back(r2.getIdentifier());
        t.addRow(r2);
    }

    Table t{"myTable"};
    TableRow r;
    std::vector<IdentifierHash> ids;
};

class DescribeTests : public testing::Test {
protected:
    void SetUp() override {
        t.addAttributes({
                {"name", DataTypes::varchar_type},
                {"birthday", DataTypes::datetime_type},
                {"age", DataTypes::int_type},
                {"weight", DataTypes::float_type},
                {"friend", DataTypes::varchar_type},
                {"married", DataTypes::bool_type}
        });

//        t.attributes["name"];
    }

    Table t{"myTable"};
    TableRow r;
};


class DefaultValueTests : public testing::Test {
protected:
    void SetUp() override {
        Attribute nameAttr{"name", DataTypes::varchar_type};
        Attribute ageAttr{"age", DataTypes::int_type};
        Attribute weightAttr{"weight", DataTypes::float_type};
        Attribute friendAttr{"friend", DataTypes::varchar_type};
        Attribute marriedAttr{"married", DataTypes::bool_type};

        nameAttr.default_value = "No name provided";
        ageAttr.default_value = 99;
        weightAttr.default_value = 99.99;
        friendAttr.default_value = "Has no friend...";
        marriedAttr.default_value = false;

        t.addAttributes({
                                nameAttr,
                                ageAttr,
                                weightAttr,
                                friendAttr,
                                marriedAttr
                        });
    }

    Table t{"myTable"};
    TableRow r;
};

TEST_F(TableTests, String) {
    EXPECT_EQ(t.cache[ids[0]].getString("name"), "Bob");
    EXPECT_EQ(t.cache[ids[0]].getString("age"), "25");
    EXPECT_EQ(t.cache[ids[0]].getString("weight"), "200.5");
    EXPECT_EQ(t.cache[ids[0]].getString("friend"), "NULL");
    EXPECT_EQ(t.cache[ids[0]].getString("married"), "False");
    EXPECT_EQ(t.cache[ids[0]].getString("not exist"), std::nullopt);
}

TEST_F(TableTests, Describe) {
    Table description = t.describe();
    EXPECT_EQ(description.cache[Helpers::hash("age")].getString("Null"), "Yes");
    TabularView view{description, 2, false};
    view.show(std::cout);
}

TEST_F(TableTests, AddRow) {
    std::string name = "A Table";
    Table t(name);

    EXPECT_EQ(t.addRow(r).error, Errors::noError);
//    EXPECT_EQ(t.addRow(r), StatusResult()) Check duplicate row
}

TEST_F(TableTests, BasicTest) {
    TabularView view{t};
    view.show(std::cout);
}

TEST_F(TableTests, RowEncode) {
    std::stringstream ss;
    EXPECT_EQ(t.cache[ids[1]].encode(ss), StatusResult{Errors::noError});
    std::string expected = "'Charlie Chaplin',5,200.5,NULL,False";
    EXPECT_EQ(ss.str(), expected);
}

TEST_F(TableTests, RowHashNoPKey) {
    IdentifierHash h1 = t.cache[ids[0]].getIdentifier();
    EXPECT_EQ(h1, t.cache[ids[0]].getIdentifier());
    IdentifierHash h2 = t.cache[ids[1]].getIdentifier();
    EXPECT_NE(h2, h1);
}

TEST_F(TableTests, UpdateRowPkey) {
    TableRow copy = t.cache[ids[0]];
    IdentifierHash h1 = copy.getIdentifier();
    EXPECT_EQ(t.cache[ids[0]].getIdentifier(), h1);
    EXPECT_EQ(*copy.getString("name"), "Bob");
    copy.updateKey("name", "Bro");

    EXPECT_EQ(*copy.getString("name"), "Bro");
    EXPECT_NE(copy.getIdentifier(), h1);
}

TEST_F(DefaultValueTests, Describe) {
    StatusResult result{};
    r = t.buildRow({"Bob", 25, 80.0}, {{"name"}, {"age"}, {"weight"}}, result);
    EXPECT_EQ(result.error, Errors::noError);
    t.addRow(r);

    Table description = t.describe();
    TabularView view{description, 2, false};
    view.show(std::cout);
}

TEST_F(DefaultValueTests, BasicTest) {
    StatusResult result{};
    std::string default_friend = "Has no friend...";
    bool        default_married = false;

    r = t.buildRow({"Bob", 25, 80}, {{"name"}, {"age"}, {"weight"}}, result);
    EXPECT_EQ(result.error, Errors::noError);
    EXPECT_EQ(r.size(), 5);
    EXPECT_EQ(std::get<std::string>(r[3]), default_friend);
    EXPECT_EQ(std::get<bool>(r[4]), default_married);
//    t.addRow(r);
//    TabularView view{t};
//    view.show(std::cout);
}


TEST(DatabaseTests, Create) {
    {
        Database db("myDB", CreateFile());
    }
    BlockIO io{"myDB", OpenFile()};
    Block b;
    io.readBlock(0, b);
    BlockHeader h(ECE141::BlockType::metadata, 0, 0, Helpers::hash("myDB"));
    h.payloadSize = 4;
    EXPECT_EQ(b.header, h);
}

TEST(DatabaseTests, Drop) {
    {
        Database db("myDB", CreateFile());
    }
    Database db("myDB", OpenFile());
    db.drop();
    EXPECT_FALSE(std::filesystem::exists(Config::getDBPath("myDB")));
}

TEST(AttrTests_withoutDefault, EncodeDecode1) {
    std::ostringstream oss(std::ios::binary);
    Attribute attr1;
    attr1.name = "testName1";
    attr1.type = DataTypes::varchar_type;
    attr1.primary_key = true;
    attr1.encode(oss);

    std::istringstream iss(oss.str());
    Attribute attr2;
    attr2.decode(iss);
    EXPECT_EQ(attr1, attr2);
}

TEST(AttrTests_withoutDefault, EncodeDecode2) {
    std::ostringstream oss(std::ios::binary);
    Attribute attr1;
    attr1.name = "testName2";
    attr1.type = DataTypes::bool_type;
    attr1.field_length = 8;
    attr1.nullable = true;
    attr1.encode(oss);

    std::istringstream iss(oss.str());
    Attribute attr2;
    attr2.decode(iss);
    EXPECT_EQ(attr1, attr2);
}

TEST(AttrTests_withoutDefault, EncodeDecode3) {
    std::ostringstream oss(std::ios::binary);
    Attribute attr1;
    attr1.name = "1999-12-24";
    attr1.type = DataTypes::datetime_type;
    attr1.field_length = 10;
    attr1.unique = true;
    attr1.encode(oss);

    std::istringstream iss(oss.str());
    Attribute attr2;
    attr2.decode(iss);
    EXPECT_EQ(attr1, attr2);
}

TEST(SchemaTests_withoutDefault, EncodeDecode1) {
    std::ostringstream oss(std::ios::binary);

    Attribute attr1;
    attr1.name = "testAttr1";
    attr1.type = DataTypes::varchar_type;
    attr1.primary_key = true;

    Schema schema1;
    schema1.name = "testSchema1";
    schema1.attributes.insert(attr1.name, attr1);
    schema1.encode(oss);

    std::istringstream iss(oss.str());
    Schema schema2;
    schema2.decode(iss);
    EXPECT_EQ(schema1, schema2);
}

TEST(SchemaTests_withoutDefault, EncodeDecode2) {
    std::ostringstream oss(std::ios::binary);

    Attribute attr1;
    attr1.name = "testAttr2";
    attr1.type = DataTypes::bool_type;
    attr1.field_length = 8;
    attr1.nullable = true;

    Schema schema1;
    schema1.name = "testSchema1";
    schema1.attributes.insert(attr1.name, attr1);
    schema1.encode(oss);

    std::istringstream iss(oss.str());
    Schema schema2;
    schema2.decode(iss);
    EXPECT_EQ(schema1, schema2);
}

TEST(SchemaTests_withoutDefault, MultipleAttrs) {
    std::ostringstream oss(std::ios::binary);

    Attribute attr1;
    attr1.name = "testAttr2";
    attr1.type = DataTypes::bool_type;
    attr1.field_length = 8;
    attr1.nullable = true;

    Attribute attr2;
    attr2.name = "abc123";
    attr2.type = DataTypes::varchar_type;
    attr2.primary_key = true;

    Schema schema1;
    schema1.name = "testSchema1";
    schema1.addAttribute(attr1);
    schema1.addAttribute(attr2);

    schema1.encode(oss);
    std::istringstream iss(oss.str());
    Schema schema2;
    schema2.decode(iss);
    EXPECT_EQ(schema1, schema2);
    EXPECT_EQ(*schema1.pkey, "abc123");
    EXPECT_EQ(*schema2.pkey, "abc123");
}

TEST(SchemaTests_withoutDefault, Fstream) {
    std::fstream file;
    file.open("testFiles/testSchema", std::fstream::out | std::ios::binary | std::fstream::trunc);

    Attribute attr1;
    attr1.name = "testAttr2";
    attr1.type = DataTypes::bool_type;
    attr1.field_length = 891283;
    attr1.nullable = true;

    Attribute attr2;
    helperTestStorable(&attr1, &attr2);

    EXPECT_EQ(attr1, attr2);
}


TEST(AttrTests_withDefault, EncodeDecode1) {
    std::ostringstream oss(std::ios::binary);
    Attribute attr1;
    attr1.name = "testName1";
    attr1.type = DataTypes::varchar_type;
    attr1.primary_key = true;
    attr1.default_value = "DefaultName001";
    attr1.encode(oss);

    std::istringstream iss(oss.str());
    Attribute attr2;
    attr2.decode(iss);
    EXPECT_EQ(attr1, attr2);
}

TEST(AttrTests_withDefault, EncodeDecode2) {
    std::ostringstream oss(std::ios::binary);
    Attribute attr1;
    attr1.name = "testName2";
    attr1.type = DataTypes::bool_type;
    attr1.field_length = 8;
    attr1.nullable = true;
    attr1.default_value = true;
    attr1.encode(oss);

    std::istringstream iss(oss.str());
    Attribute attr2;
    attr2.decode(iss);
    EXPECT_EQ(attr1, attr2);
}

TEST(SchemaTests_withDefault, EncodeDecode1) {
    std::ostringstream oss(std::ios::binary);

    Attribute attr1;
    attr1.name = "testAttr1";
    attr1.type = DataTypes::varchar_type;
    attr1.primary_key = true;
    attr1.default_value = "DefaultString";

    Schema schema1;
    schema1.name = "testSchema1";
    schema1.attributes.insert(attr1.name, attr1);
    schema1.encode(oss);

    std::istringstream iss(oss.str());
    Schema schema2;
    schema2.decode(iss);
    EXPECT_EQ(schema1, schema2);
}