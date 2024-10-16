//
//  Filters.cpp
//  Datatabase5
//
//  Created by rick gessner on 3/5/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//



#include <string>
#include <limits>
#include "misc/Filters.hpp"
#include "tokenizer/keywords.hpp"
#include "misc/Helpers.hpp"
#include "database/Schema.hpp"
//#include "database/Attribute.hpp"
#include "misc/ParseHelper.hpp"

namespace ECE141 {

    Filters::OpMap Filters::comparitors = {
            {Operators::equal_op,    equals},
            {Operators::notequal_op, notEquals},
            {Operators::lt_op,       lessThan},
            {Operators::gt_op,       greaterThan},
            {Operators::lte_op,      lessEqual},
            {Operators::gte_op,      greaterEqual}
    };


    Filters::ComparableValue convertToSubsetVariant(const Value & var) {
        if (std::holds_alternative<int>(var)) {
            return {std::get<int>(var)};
        }
        if (std::holds_alternative<double>(var)) {
            return {std::get<double>(var)};
        }
        if (std::holds_alternative<std::string>(var)) {
            return {std::get<std::string>(var)};
        }
        if (std::holds_alternative<bool>(var)) {
            return {std::get<bool>(var)};
        }
        return {false};
    }

    bool Expression::operator()(const Row &aRow) {
        lhs.evaluate(aRow);
        rhs.evaluate(aRow);
        if (!lhs.couldEvaluate || !rhs.couldEvaluate) return true;
        // We can't fully evaluate this expression right now,
        // (eg looking at an individual TableRow, before we have Joined this row, but need elements from both)

        // Now we have values in lhs/rhs

        if (std::holds_alternative<Null>(lhs.value) || std::holds_alternative<Null>(rhs.value) ||
            std::holds_alternative<AutoIncrementer>(lhs.value) || std::holds_alternative<AutoIncrementer>(rhs.value)) {
            if (lhs == rhs) {
                return op == Operators::equal_op || op == Operators::lte_op || op == Operators::gte_op;
            }
            return false;
        }

        Filters::ComparableValue cLhs = convertToSubsetVariant(lhs.value);
        Filters::ComparableValue cRhs = convertToSubsetVariant(rhs.value);
        bool result = Filters::comparitors[op](cLhs, cRhs);
        return result;
    }

    //--------------------------------------------------------------

    Filters::Filters() = default;
    Filters::Filters(const Filters &aCopy) = default;
    Filters::~Filters() = default;

    Filters &Filters::add(const Expression& anExpression) {
        expressions.push_back(anExpression);
        return *this;
    }

    //compare expressions to row; return true if matches
    bool Filters::matches(const Row &aRow) {
        for (auto &theExpr : expressions) {
            if (theExpr.logic == Logical::unknown_op) { return false; }
            if (!theExpr(aRow)) {
                if (theExpr.logic == Logical::and_op || theExpr.logic == Logical::no_op) { return false; }
            } else if (theExpr.logic == Logical::or_op) { return true; }
        }
        return true;
    }

#if false

    //compare expressions to row; return true if matches
    bool Filters::matches(KeyValues &aList) const {

        //STUDENT: You'll need to add code here to deal with
        //         logical combinations (AND, OR, NOT):
        //         like:  WHERE zipcode=92127 AND age>20

//    for(auto &theExpr : expressions) {
//      if(!(*theExpr)(aList)) {
//        return false;
//      }
//    }
//    return true;
    }


    //where operand is field, number, string...
    StatusResult parseOperand(Tokenizer &aTokenizer,
                              Schema &aSchema, Operand &anOperand) {
//    StatusResult theResult{noError};
//    Token &theToken = aTokenizer.current();
//    if(TokenType::identifier==theToken.type) {
//      if(auto *theAttr=aSchema.getAttribute(theToken.data)) {
//        anOperand.ttype=theToken.type;
//        anOperand.name=theToken.data; //hang on to name...
//        anOperand.schemaId=aSchema::hashString(theToken.data);
//        anOperand.dtype=theAttr->getType();
//      }
//      else {
//        anOperand.ttype=TokenType::string;
//        anOperand.dtype=DataTypes::varchar_type;
//        anOperand.value=theToken.data;
//      }
//    }
//    else if(TokenType::number==theToken.type) {
//      anOperand.ttype=TokenType::number;
//      anOperand.dtype=DataTypes::int_type;
//      if (theToken.data.find('.')!=std::string::npos) {
//        anOperand.dtype=DataTypes::float_type;
//        anOperand.value=std::stof(theToken.data);
//      }
//      else anOperand.value=std::stoi(theToken.data);
//    }
//    else theResult.error=syntaxError;
//    if(theResult) aTokenizer.next();
//    return theResult;
    }

    //STUDENT: Add validation here...
    bool validateOperands(Operand &aLHS, Operand &aRHS, Schema &aSchema) {
        if (TokenType::identifier == aLHS.ttype) { //most common case...
            //STUDENT: Add code for validation as necessary
            for (const auto &attr: aSchema.attributes) {
                if (attr.first == Helpers::valueToString(aLHS.value, DataTypes::varchar_type)) {
                    return true;
                }
            }
            return true;
        } else if (TokenType::identifier == aRHS.ttype) {
            //STUDENT: Add code for validation as necessary
            for (const auto &attr: aSchema.attributes) {
                if (attr.first == Helpers::valueToString(aRHS.value, DataTypes::varchar_type)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isValidOperand(Token &aToken) {
        //identifier, number, string...
        if (aToken.type == TokenType::identifier ||
            aToken.type == TokenType::number ||
            aToken.type == TokenType::string)
            return true;
        return false;
    }

    //STUDENT: This starting point code may need adaptation...
    StatusResult Filters::parse(Tokenizer &aTokenizer, Schema &aSchema) {
        StatusResult theResult;
        ParseHelper theHelper(aTokenizer);
        while (theResult && (2 < aTokenizer.remaining())) {
            if (isValidOperand(aTokenizer.current())) {
                Expression theExpr;
                if ((theResult = theHelper.parseExpression(aSchema, theExpr))) {
                    expressions.push_back(theExpr);
                    //add logic to deal with bool combo logic...
                }
            } else break;
        }
        return theResult;
    }
#endif

}
