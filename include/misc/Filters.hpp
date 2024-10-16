//
//  Filters.hpp
//  RGAssignment5
//
//  Created by rick gessner on 4/4/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#ifndef Filters_h
#define Filters_h

#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <variant>
#include "tokenizer/Tokenizer.hpp"
#include "database/TableRow.hpp"
#include "Helpers.hpp"
#include "Compare.hpp"


namespace ECE141 {

    class Schema;

    using KeyValues = std::map<std::string, Value>;

    struct Operand {
        Operand() {}

        Operand(TokenType aType, Value aVal)
                : ttype(aType), dtype(DataTypes::varchar_type), value(aVal) {}

        Operand(TokenType aType, TableField &aTableField)
                : ttype(aType), dtype(DataTypes::varchar_type), value(Null()), tableField(aTableField) {}

        Operand(TokenType aType, Value &aValue, TableField &aTableField)
                : ttype(aType), dtype(DataTypes::varchar_type), value(aValue), tableField(aTableField) {}

        bool operator==(const Operand &aOperand) const {
            return ttype == aOperand.ttype && dtype == aOperand.dtype && value == aOperand.value &&
                   tableField == aOperand.tableField;
        }

        void setNumber(const Token &aToken) {
            ttype = aToken.type;
            dtype = DataTypes::float_type;
            if (Helpers::isInt(aToken.data)) { dtype = DataTypes::int_type; }
            value = Helpers::stringToValue(aToken.data, dtype);
        }

        void setVarChar(const Token &aToken) {
            ttype = aToken.type;
            dtype = DataTypes::varchar_type;
            value = Helpers::stringToValue(aToken.data, dtype);
        }

        void setField(const TableField &aTableField) {
            ttype = TokenType::identifier;
            dtype = DataTypes::varchar_type;
            value = aTableField.fieldName;
            tableField = aTableField;
        }

        void setBool(const Token &aToken) {
            ttype = TokenType::keyword;
            dtype = DataTypes::bool_type;
            value = Helpers::stringToValue(aToken.data, dtype);
        }

        void evaluate(const Row& aRow) {
            if (ttype == TokenType::identifier) {
                if (auto val = aRow.getItem(tableField)) {
                    value = *val;
                    couldEvaluate = true;
                } else {
                    couldEvaluate = false;
                }
            }
        }

        TokenType ttype; //is it a field, or const (number, string)...
        DataTypes dtype;
        Value value;
        TableField tableField;
        bool couldEvaluate = true;
    };

    //---------------------------------------------------

    struct Expression {
        Operand lhs;  //id
        Operand rhs;  //usually a constant; maybe a field...
        Operators op;   //=     //users.id=books.author_id
        Logical logic; //and, or, not...

        Expression(const Operand &aLHSOperand = {},
                   const Operators anOp = Operators::unknown_op,
                   const Operand &aRHSOperand = {})
                : lhs(aLHSOperand), rhs(aRHSOperand),
                  op(anOp), logic(Logical::no_op) {}

        Expression(const Expression &anExpr) :
                lhs(anExpr.lhs), rhs(anExpr.rhs),
                op(anExpr.op), logic(anExpr.logic) {}

        bool operator==(const Expression &anExpr) const {
            return lhs == anExpr.lhs && rhs == anExpr.rhs && op == anExpr.op && logic == anExpr.logic;
        }

        bool operator()(const Row &);

    };

    using Expressions = std::vector<Expression>;

    //---------------------------------------------------

    class Filters {
    public:

        Filters();
        Filters(const Filters &aFilters);
        ~Filters();

        size_t getCount() const { return expressions.size(); }

        bool matches(const Row& aRow);

        Filters &add(const Expression& anExpression);

        Expressions &getExpressions() { return expressions; }

        using ComparableValue = std::variant<bool, int, double, std::string>;
        using Comparitor = std::function<bool(ComparableValue &aLHS, ComparableValue &aRHS)>;

        using OpMap = std::map<Operators, Comparitor>;
        static OpMap comparitors;

    protected:
        inline static bool equals(ComparableValue &aLHS, ComparableValue &aRHS) {
            bool theResult = false;
            std::visit([&](auto const &aLeft) {
                std::visit([&](auto const &aRight) {
                    theResult = isEqual(aLeft, aRight);
                }, aRHS);
            }, aLHS);
            return theResult;
        }

        inline static bool lessThan(ComparableValue &aLHS, ComparableValue &aRHS) {
            bool theResult = false;
            std::visit([&](auto const &aLeft) {
                std::visit([&](auto const &aRight) {
                    theResult = isLessThan(aLeft, aRight);
                }, aRHS);
            }, aLHS);
            return theResult;
        }

        inline static bool notEquals(ComparableValue &aLHS, ComparableValue &aRHS) {
            bool theResult = false;
            theResult = !equals(aLHS, aRHS);
            return theResult;
        }

        inline static bool greaterThan(ComparableValue &aLHS, ComparableValue &aRHS) {
            bool theResult = false;
            theResult = !(lessThan(aLHS, aRHS) || equals(aLHS, aRHS));
            return theResult;
        }

        inline static bool lessEqual(ComparableValue &aLHS, ComparableValue &aRHS) {
            bool theResult = false;
            theResult = !greaterThan(aLHS, aRHS);
            return theResult;
        }

        inline static bool greaterEqual(ComparableValue &aLHS, ComparableValue &aRHS) {
            bool theResult = false;
            theResult = !lessThan(aLHS, aRHS);
            return theResult;
        }

        Expressions expressions;
    };

}

#endif /* Filters_h */