//
// Created by Daniel Ziper on 5/5/24.
//

#include <memory>
#include "gtest/gtest.h"
#include "TestHelpers.hpp"
#include "../include/database/Database.hpp"
#include "../include/parsing/TokenParser.hpp"
#include "../include/parsing/CreateTableParser.hpp"
#include "../include/misc/Logger.hpp"

using namespace ECE141;

class IntegrationTests : public testing::Test {
protected:
    void createTable(std::shared_ptr<Database> db, const std::string& command) {
        CreateTableParser creater(db);
        std::stringstream ss;
        ss << command;
        Tokenizer tok(ss);
        tok.tokenize();
        TokenParser parser(tok);
        EXPECT_TRUE(parser.parseWith(creater));
        EXPECT_EQ(db->createTable(std::move(creater.table)).error, Errors::noError);
    }

    void SetUp() override {
        {
            std::shared_ptr<Database> db = std::make_shared<Database>(dbname, CreateFile());
            db->setSharedPtr(db);

            createTable(db, "create table users (id int primary key, name varchar, weight float)");
            createTable(db, "create table another (name varchar unique not null, age int not null, weight float, friend varchar(10) unique)");

            Table* tb = db->getTable("users");
            tb->addRow({tb, values[0]});
            tb->addRow({tb, values[1]});
            StatusResult err;
            // Build this row, it has weird float value.
            tb->addRow(
                    tb->buildRow(values[2], {{"id"}, {"name"}, {"weight"}}, err)
            );
            EXPECT_EQ(err.error, Errors::noError);
        }
        {
            loaded = std::make_shared<Database>(dbname, OpenFile());
            loaded->setSharedPtr(loaded);
        }
    };
    std::string dbname = "integrationTest";
    std::shared_ptr<Database> loaded;
    std::vector<std::vector<Value>> values {
            {5, "bob", 123.3},
            {3, "billy", 645.3},
            {20, "joe", 645.0}
    };
};

TEST_F(IntegrationTests, LoadDB) {
    Table* table = loaded->getTable("users");
    EXPECT_TRUE(table);
//    Table description = table->describe();
    EXPECT_TRUE(table->attributes.get("id")->primary_key);
}

TEST_F(IntegrationTests, LoadTable) {
    Table* table = loaded->getTable("users");
    table->loadAllRows();
    TableRow& row = table->cache.getRow(3);
    EXPECT_EQ(row.getValues(), values[1]);
    row = table->cache.getRow(5);
    EXPECT_EQ(row.getValues(), values[0]);
}

TEST_F(IntegrationTests, LoadIntLikeDouble) {
    Table* table = loaded->getTable("users");
    table->loadAllRows();
    TableRow& row = table->cache.getRow(20);
    EXPECT_EQ(row.getValues(), values[2]);
}


TEST_F(IntegrationTests, Validation) {
    Table* table = loaded->getTable("another");
    StatusResult err;
    TableFieldList fields = {{"name"}, {"age"}, {"weight"}, {"friend"}};
    TableRow r = table->buildRow({"Bob", 4, 12, Null()}, fields, err);
    EXPECT_SUCC(err);
    EXPECT_SUCC(table->addRow(r));
    EXPECT_EQ(r.size(), 4);
    EXPECT_EQ(table->size(), 1);

    r = table->buildRow({"Bob", 5, 100.5, Null()}, fields, err);
    EXPECT_SUCC(err);
    err = table->addRow(r);
    EXPECT_ERR(err, Errors::keyExists);

    r = table->buildRow({"Bobert", 5, 10, Null()}, fields, err);
    EXPECT_SUCC(err);
    EXPECT_TRUE(std::holds_alternative<double>(r[2]));  // Int converted to Double
    err = table->addRow(r);
    EXPECT_SUCC(err);  // Null() not checked for uniqueness

    err = {Errors::noError};
    r = table->buildRow({"Bobert The Second", 5, 100.5, "A very long name"}, fields, err);
    EXPECT_ERR(err, Errors::lengthMismatch);
    EXPECT_EQ(r.size(), 0);

    err = {Errors::noError};
    r = table->buildRow({"Bobert The Third", Null(), 100.5, "Hi"}, fields, err);
    EXPECT_ERR(err, Errors::notNullable);

    err = {Errors::noError};
    r = table->buildRow({"Bobert The Third", 100, 100.5, "Hi"}, fields, err);
    EXPECT_SUCC(err);
    err = table->addRow(r);
    EXPECT_SUCC(err);

    EXPECT_EQ(table->size(), 3);
}


TEST(Logging, Log) {
    auto& l = Logger::getInstance();
    Logger::setLevel(LogLevel::Trace);
    l.log(LogLevel::Error, "Hi");
    Logger::log(LogLevel::Warning, "Hello");
    Logger::log(LogLevel::Error, "Bye");

    ERROR("1234", 43);
    TRACE("Hello this is a trace")
}