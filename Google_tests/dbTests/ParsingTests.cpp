//
// Created by Daniel Ziper on 4/21/24.
//

#include <gtest/gtest.h>

#include "../include/parsing/TokenParser.hpp"
#include "../include/database/Schema.hpp"
#include "../include/parsing/AttributeParser.hpp"
#include "../include/parsing/CreateTableParser.hpp"
#include "../include/parsing/ValueParser.hpp"
#include "../include/misc/BasicTypes.hpp"
#include "../include/parsing/MultiParser.hpp"
#include "../include/parsing/InsertParser.hpp"
#include "TestHelpers.hpp"
#include "parsing/SelectParser.hpp"
#include "parsing/UpdateParser.hpp"
#include "parsing/DeleteParser.hpp"

using namespace ECE141;

TEST(TokenParserTests, Basic) {
    std::stringstream ss;
    ss << "create database users (";
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    std::string name;
    EXPECT_TRUE(parser.clear().nextIs({Keywords::create_kw, Keywords::database_kw}));
    EXPECT_TRUE(parser.getIdentifier(name));
    EXPECT_TRUE(parser.skipPast(left_paren));
    EXPECT_EQ(name, "users");
}

class AttributeParsingTests : public testing::Test {
protected:
    void SetUp() override {
       query << "CREATE TABLE tasks (\n"
            << "id INT AUTO_INCREMENT PRIMARY KEY,\n"
            << "title VARCHAR(100) NOT NULL,\n"
            << "price FLOAT DEFAULT 0.0,\n"
            << "due_date TIMESTAMP,\n"
            << "status BOOLEAN DEFAULT FALSE,\n"
            << ")";

       Attribute a1{"id", DataTypes::int_type};
       a1.auto_increment = true;
       a1.primary_key = true;
       a1.unique = true;
       attrs.push_back(a1);
       attrsNoDefault.push_back(a1);

       Attribute a2{"title", DataTypes::varchar_type};
       a2.field_length = 100;
       a2.nullable = false;
       attrs.push_back(a2);
       attrsNoDefault.push_back(a2);

       Attribute a3 {"price", DataTypes::float_type};
       attrsNoDefault.push_back(a3);
       a3.default_value = {0.0f};
       attrs.push_back(a3);

       Attribute a4 {"due_date", DataTypes::float_type};
       attrs.push_back(a4);
       attrsNoDefault.push_back(a4);

       Attribute a5 {"status", DataTypes::bool_type};
       attrsNoDefault.push_back(a5);
       a5.default_value = {false};
       attrs.push_back(a5);
    }

    std::string getLine(std::stringstream& aQuery, size_t num) {
        std::string l;
        num++;
        while (num > 0) {
            std::getline(aQuery, l);
            num--;
        }
        return l;
    }

    std::stringstream query;
    std::vector<Attribute> attrs;
    std::vector<Attribute> attrsNoDefault;
};

TEST_F(AttributeParsingTests, Basic) {
    std::stringstream ss;
    ss << getLine(query, 4);
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    AttributeParser attributeParser;
    ASSERT_TRUE(parser.parseWith(attributeParser));
    ASSERT_EQ(attributeParser.out, attrs[3]);
}

TEST_F(AttributeParsingTests, NoDefaults) { // We can parse default value now
    std::string line;
    std::getline(query, line); // discard first line
    for (auto& attr : attrs) {
        std::getline(query, line);
        std::stringstream ss{line};

        Tokenizer t{ss};
        t.tokenize();
        TokenParser parser{t};

        AttributeParser attributeParser;
        ASSERT_TRUE(parser.parseWith(attributeParser));
        ASSERT_EQ(attributeParser.out, attr);
    }
}

TEST_F(AttributeParsingTests, SchemaNoDefaults) {
    Tokenizer t{query};
    t.tokenize();
    TokenParser parser{t};
    CreateTableParser createTableParser;
    EXPECT_TRUE(parser.parseWith(createTableParser));

    Schema& schema = createTableParser.table;
    EXPECT_EQ(schema.name, "tasks");

    size_t count = 0;
    schema.attributes.orderedEach([&](size_t index, const std::string& key, const Attribute& attr) {
        EXPECT_EQ(attr, attrs[index]);
        count++;
        return true;
    });
    EXPECT_EQ(count, attrs.size());
}

TEST_F(AttributeParsingTests, CreateError) {
    std::stringstream ss;
    ss << "create  TABLE  table_c  id  int NOT NULL auto_increment primary key , name  varchar(50) , price  float );";

    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};
    CreateTableParser createTableParser;
    StatusResult theResult = parser.parseWith(createTableParser).getState();

    EXPECT_EQ(theResult.error, Errors::punctuationExpected);
}

TEST(ValueParsingTests, knownType) {
    std::stringstream ss;
    ss << "\'Bob\' 5 2.42 NULL True False create dddd.404" ;
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};
    ValueParser valueParser;

    StatusResult theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<std::string>(&valueParser.out), "Bob");

    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<int>(&valueParser.out), 5);

    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<double>(&valueParser.out), 2.42);

    Null null;
    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<NULL>(&valueParser.out), null);

    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<bool>(&valueParser.out), true);

    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<bool>(&valueParser.out), false);

    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(theResult.error, Errors::valueExpected);
}

TEST(ValueParsingTests, DoubleQuote) {
    // Note: This doesn't follow the SQL standard, but looks like the autograder is using
    // Double quotes instead of single quotes for strings...

    std::stringstream ss;
    ss << "\"Bob\" \"Testing 123\"" ;
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};
    ValueParser valueParser;

    StatusResult theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<std::string>(&valueParser.out), "Bob");

    theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(*std::get_if<std::string>(&valueParser.out), "Testing 123");
}

TEST(ValueParsingTests, unknownType) {
    std::stringstream ss;
    ss << "dddd.404" ;
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};
    ValueParser valueParser;

    StatusResult theResult = parser.parseWith(valueParser).getState();
    EXPECT_EQ(theResult.error, Errors::unknownType);
}

TEST(MultiParserTests, Basic) {
    std::stringstream ss;
    ss << "(\'Bob\', 5, 2.42, NULL ,True,False)";
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    ValueParser vp;
    MultiParser<Value> mp(vp);
    StatusResult theResult = parser.parseWith(mp).getState();

    std::vector<Value> expected = {"Bob", 5, 2.42, Null(), true, false};
    EXPECT_EQ(theResult.error, Errors::noError);
    EXPECT_EQ(mp.out, expected);
}

TEST(MultiParserTests, MultiMulti) {
    std::stringstream ss;
    ss << "(\'Bob\', 5, 2.42, NULL), ('hello', 123, True)";

    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    CommaSepValueParser csvP;
    MultiParser<std::vector<Value>> multiMultiValuesParser(csvP, ",", "", "");

    StatusResult theResult = parser.parseWith(multiMultiValuesParser).getState();
    std::vector<Value> exp0 = {"Bob", 5, 2.42, Null()};
    std::vector<Value> exp1 = {"hello", 123, true};

    EXPECT_EQ(theResult.error, Errors::noError);
    EXPECT_EQ(multiMultiValuesParser.out[0], exp0);
    EXPECT_EQ(multiMultiValuesParser.out[1], exp1);
}

TEST(InsertParserTests, Basic) {
    std::stringstream ss;
    ss << R"(INSERT INTO Users (first_name, last_name, age, zipcode) VALUES ("Terry",     "Pratchett", 70,  92124), ("Ian",       "Tregellis", 48,  92123), ("Jody",      "Taylor",    50,  92120), ("Stephen",   "King",      74,  92125), ("Ted",       "Chiang",    56,  92120))";

    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    InsertParser insertParser;
    StatusResult theResult = parser.parseWith(insertParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);


    TableFieldList expFields{{"first_name"}, {"last_name"}, {"age"}, {"zipcode"}};
    std::vector<Value> expVal0 = {"Terry", "Pratchett", 70, 92124};
    std::vector<Value> expVal1 = {"Ian", "Tregellis", 48, 92123};
    std::vector<Value> expVal2 = {"Jody", "Taylor", 50, 92120};
    std::vector<Value> expVal3 = {"Stephen", "King", 74, 92125};
    std::vector<Value> expVal4 = {"Ted", "Chiang", 56, 92120};

    EXPECT_EQ(insertParser.tableName, "Users");
    EXPECT_EQ(insertParser.fields, expFields);
    EXPECT_EQ(insertParser.values[0], expVal0);
    EXPECT_EQ(insertParser.values[1], expVal1);
    EXPECT_EQ(insertParser.values[2], expVal2);
    EXPECT_EQ(insertParser.values[3], expVal3);
    EXPECT_EQ(insertParser.values[4], expVal4);
    EXPECT_EQ(insertParser.values.size(), 5);
};

TEST(ClauseParserTests, Limit1) {
    std::stringstream ss;
    ss << "Limit 5";
    DBQuery theQuery;
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    ClauseParser clauseParser{theQuery};
    StatusResult theResult = parser.parseWith(clauseParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    EXPECT_EQ(theQuery.limits.size(), 1);
    EXPECT_EQ(theQuery.limits[0], 5);
}

TEST(ClauseParserTests, Limit2) {
    std::stringstream ss;
    ss << "Limit 5, 20 OtherCommand";
    Tokenizer t{ss};
    t.tokenize();
    DBQuery theQuery;
    TokenParser parser{t};

    ClauseParser clauseParser{theQuery};
    StatusResult theResult = parser.parseWith(clauseParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    EXPECT_EQ(theQuery.limits.size(), 2);
    EXPECT_EQ(theQuery.limits[0], 5);
    EXPECT_EQ(theQuery.limits[1], 20);
}

TEST(ClauseParserTests, GroupBy1) {
    std::stringstream ss;
    ss << "Group by name";
    Tokenizer t{ss};
    t.tokenize();
    DBQuery theQuery;
    TokenParser parser{t};

    ClauseParser clauseParser{theQuery};
    StatusResult theResult = parser.parseWith(clauseParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableField tf{"name"};
    EXPECT_EQ(theQuery.groupByFields.size(), 1);
    EXPECT_EQ(theQuery.groupByFields[0], tf);
}

TEST(ClauseParserTests, GroupBy2) {
    std::stringstream ss;
    ss << "Group by name, age OtherCommand";
    Tokenizer t{ss};
    t.tokenize();
    DBQuery theQuery;
    TokenParser parser{t};

    ClauseParser clauseParser{theQuery};
    StatusResult theResult = parser.parseWith(clauseParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableField tf1{"name"};
    TableField tf2{"age"};
    EXPECT_EQ(theQuery.groupByFields.size(), 2);
    EXPECT_EQ(theQuery.groupByFields[0], tf1);
    EXPECT_EQ(theQuery.groupByFields[1], tf2);
}

TEST(ClauseParserTests, OrderBy1) {
    std::stringstream ss;
    ss << "Order by name, age Others";
    Tokenizer t{ss};
    t.tokenize();
    DBQuery theQuery;
    TokenParser parser{t};

    ClauseParser clauseParser{theQuery};
    StatusResult theResult = parser.parseWith(clauseParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableField tf1{"name"};
    TableField tf2{"age"};
    EXPECT_EQ(theQuery.orderByFields.size(), 2);
    EXPECT_EQ(theQuery.orderByFields[0].field, tf1);
    EXPECT_EQ(theQuery.orderByFields[1].field, tf2);
}

TEST(ExpressionParserTests, compareNum) {
    std::stringstream ss;
    ss << "age < 20";

    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    ExpressionParser expreParser;
    StatusResult theResult = parser.parseWith(expreParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    Value age = "age";
    Value twenty = 20;
    TableField ageField{"age", "", "age"};
    TableField emptyField;
    Operand lhs{TokenType::identifier, age, ageField};
    Operand rhs{TokenType::number, twenty, emptyField};
    rhs.dtype = DataTypes::int_type;
    Operators op{Operators::lt_op};
    Expression expectedExp{lhs, op, rhs};

    EXPECT_EQ(expectedExp, expreParser.out);
}

TEST(ExpressionParserTests, compareStr) {
    std::stringstream ss;
    ss << "name = 'Chris'";
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    ExpressionParser expreParser;
    StatusResult theResult = parser.parseWith(expreParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    Value name = "name";
    Value chris = "Chris";
    TableField nameField{"name", "", "name"};
    TableField emptyField;
    Operand lhs{TokenType::identifier, name, nameField};
    Operand rhs{TokenType::string, chris, emptyField};
    Operators op{Operators::equal_op};
    Expression expectedExp{lhs, op, rhs};

    EXPECT_EQ(expectedExp, expreParser.out);
}

TEST(SelectParserTests, selectAll) {
    std::stringstream ss;
    ss << "Select * from Users";
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    SelectParser selectParser;
    StatusResult theResult = parser.parseWith(selectParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);
    EXPECT_EQ(selectParser.theQuery.getAll(), true);
}

TEST(SelectParserTests, basicSelect) {
    std::stringstream ss;
    ss << "Select name, age from Users";
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    SelectParser selectParser;
    StatusResult theResult = parser.parseWith(selectParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableField tf1{"name"};
    TableField tf2{"age"};
    EXPECT_EQ(selectParser.theQuery.getAll(), false);
    EXPECT_EQ(selectParser.theQuery.selectedFields[0], tf1);
    EXPECT_EQ(selectParser.theQuery.selectedFields[1], tf2);
}

TEST(SelectParserTests, multiCommand) {
    std::stringstream ss;
    ss << "SELECT name, age FROM Users WHERE name = 'Chris' AND age > 20 LIMIT 2,5 GROUP BY zipcode ORDER BY age";
    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    SelectParser selectParser;
    StatusResult theResult = parser.parseWith(selectParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableField tf1{"name"};
    TableField tf2{"age"};
    TableField tf3{"zipcode"};
    TableField tf4{"age"};
//    Expression ex1{TokenType::string, tf5};
    EXPECT_EQ(selectParser.theQuery.getAll(), false);
    EXPECT_EQ(selectParser.theQuery.selectedFields[0], tf1);
    EXPECT_EQ(selectParser.theQuery.selectedFields[1], tf2);
    EXPECT_EQ(selectParser.theQuery.limits[0], 2);
    EXPECT_EQ(selectParser.theQuery.limits[1], 5);
    EXPECT_EQ(selectParser.theQuery.groupByFields[0], tf3);
    EXPECT_EQ(selectParser.theQuery.orderByFields[0].field, tf4);
}




TEST(UpdateTests, basic) {
    std::stringstream ss;
    ss << "UPDATE Users SET a.zipcode = 92127, b.name = 'Chris' WHERE c.versionId > 1.5";

    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    DBQuery theQuery;
    UpdateParser updateParser{theQuery};
    StatusResult theResult = parser.parseWith(updateParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableName tableName{"Users"};
    EXPECT_EQ(updateParser.theQuery.tableNames[0], tableName);

    // lhs1
    Value zipcode = "zipcode";
    TableField zipcodeField{"zipcode", "a", "a.zipcode"};
    Operand lhs1{TokenType::identifier, zipcode, zipcodeField};
    // rhs1
    Value number = 92127;
    TableField emptyField;
    Operand rhs1{TokenType::number, number, emptyField};
    rhs1.dtype = DataTypes::int_type;
    // op
    Operators eq_op{Operators::equal_op};

    // lhs2
    Value name = "name";
    TableField nameField{"name", "b", "b.name"};
    Operand lhs2{TokenType::identifier, name, nameField};
    // rhs2
    Value chris = "Chris";
    Operand rhs2{TokenType::string, chris, emptyField};
    rhs2.dtype = DataTypes::varchar_type;

    // lhs3
    Value versionId = "versionId";
    TableField versionField{"versionId", "c", "c.versionId"};
    Operand lhs3{TokenType::identifier, versionId, versionField};
    // rhs3
    Value onePointFive = 1.5;
    Operand rhs3{TokenType::number, onePointFive, emptyField};
    rhs3.dtype = DataTypes::float_type;

    Operators gt_op{Operators::gt_op};

    Expression eSetExpression1{lhs1, eq_op, rhs1};
    Expression eSetExpression2(lhs2, eq_op, rhs2);
    Expression eWhereExpression1(lhs3, gt_op, rhs3);
    DBQuery eQuery;
    eQuery.setExpressions.push_back(eSetExpression1);
    eQuery.setExpressions.push_back(eSetExpression2);
    eQuery.getFilterExpressions().push_back(eWhereExpression1);
    EXPECT_EQ(theQuery.setExpressions, eQuery.setExpressions);
    EXPECT_EQ(theQuery.getFilterExpressions(), eQuery.getFilterExpressions());
}

TEST(UpdateTests, PA7) {
    std::stringstream ss;
    ss << "update Users set zip=30587 where id=5;";
    QuickParser parser{ss};
    DBQuery query;
    UpdateParser up{query};
    ASSERT_TRUE(parser.parseWith(up));
    EXPECT_EQ(up.theQuery.tableNames[0].table, "Users");
    EXPECT_EQ(query.getFilterExpressions()[0].lhs.tableField.fieldName, "id");
    EXPECT_EQ(query.setExpressions[0].lhs.tableField.fieldName, "zip");
    EXPECT_EQ(query.setExpressions[0].rhs.value, Value{30587});
}

TEST(DeleteTests, basic) {
    std::stringstream ss;
    ss << "DELETE FROM Users WHERE age > 30 AND married = True";

    Tokenizer t{ss};
    t.tokenize();
    TokenParser parser{t};

    DBQuery theQuery;
    DeleteParser deleteParser{theQuery};
    StatusResult theResult = parser.parseWith(deleteParser).getState();
    EXPECT_EQ(theResult.error, Errors::noError);

    TableName tableName{"Users"};
    EXPECT_EQ(deleteParser.theQuery.tableNames[0], tableName);

    // lhs1
    Value age = "age";
    TableField ageField{"age", ""};
    Operand lhs1{TokenType::identifier, age, ageField};
    // rhs1
    Value number = 30;
    TableField emptyField;
    Operand rhs1{TokenType::number, number, emptyField};
    rhs1.dtype = DataTypes::int_type;
    // gt_op
    Operators gt_op{Operators::gt_op};

    // lhs2
    Value married = "married";
    TableField marriedField{"married", ""};
    Operand lhs2{TokenType::identifier, married, marriedField};
    // rhs2
    Value isMarried = true;
    Operand rhs2{TokenType::keyword, isMarried, emptyField};
    rhs2.dtype = DataTypes::bool_type;
    // eq_op
    Operators eq_op{Operators::equal_op};

    Expression eWhereExpression1(lhs1, gt_op, rhs1);
    eWhereExpression1.logic = Logical::and_op;
    Expression eWhereExpression2(lhs2, eq_op, rhs2);
    DBQuery eQuery;
    eQuery.getFilterExpressions().push_back(eWhereExpression1);
    eQuery.getFilterExpressions().push_back(eWhereExpression2);
    EXPECT_EQ(theQuery.getFilterExpressions(), eQuery.getFilterExpressions());
}
