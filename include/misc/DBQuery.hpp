//
//  DBQuery.hpp
//  PA5
//
//  Created by rick gessner on 4/7/23.
//

#ifndef DBQuery_h
#define DBQuery_h

#include "database/Schema.hpp"
#include "misc/Filters.hpp"
#include "database/Table.hpp"
#include "statement/ExecutionContext.hpp"

namespace ECE141 {

    class DBQuery {
    public:

        DBQuery(Schema *aSchema = nullptr, bool allFields = true)
                : fromTable(aSchema), all(allFields) {}

        DBQuery(const DBQuery &aQuery) : fromTable(aQuery.fromTable) {}

        std::vector<Table *> table;
        std::vector<TableName> tableNames;
        std::vector<size_t> limits;
        std::vector<OrderByField> orderByFields;
        TableFieldList selectedFields;
        TableFieldList groupByFields;
        Expressions setExpressions;
        std::vector<Keywords> joinKeywords;
        Filters joinOn;

    void addExpression(const Expression& anExpression) {
        filters.add(anExpression);
    }
    void addExpressions(const Expressions& anExpressions) {
        for (const auto& expr : anExpressions) {
            addExpression(expr);
        }
    }

    void setSchema(Schema* aSchema) { fromTable = aSchema; }
    void setAll(bool isAll) { all = isAll; }
    bool getAll() const { return all; }
    //maybe joins?

    bool matches(const TableRow &aRow) {
        return filters.matches(aRow);
    }

    Expression& getLastFilterExpression() {
        return filters.getExpressions().back();
    }

    Expressions& getFilterExpressions() {
        return filters.getExpressions();
    }

    StatusResult resolveTablesAndFields(ExecutionContext& aContext) {
        INFO("resolveTablesAndFields(ExecutionContext& aContext)")
        for (auto& name : tableNames) {
            Table* t = aContext.database->getTable(name.table);
            if (!t) {
                ERROR("unknownTable")
                return {Errors::unknownTable};
            }
            table.push_back(t);
            DEBUG("table.push_back(t), where t->name=", t->name)
        }

        // Fill up the field.table
        for (auto& orderByField : orderByFields) {
            returnIfError(resolveField(orderByField.field));
        }
        for (auto& tableField : groupByFields) {
            returnIfError(resolveField(tableField));
        }
        for (auto& tableField : selectedFields) {
            returnIfError(resolveField(tableField));
        }
        for (auto& expr : filters.getExpressions()) {
            if (expr.lhs.ttype == TokenType::identifier)
                returnIfError(resolveField(expr.lhs.tableField))
            if (expr.rhs.ttype == TokenType::identifier)
                returnIfError(resolveField(expr.rhs.tableField))
        }
        return {Errors::noError};
    }

  protected:
    Schema*  fromTable;
    Filters  filters;
    bool     all; //if user used SELECT * FROM...

  private:
    StatusResult resolveField(TableField& field) {
        for (auto t : table) {
            // Prefilled Table name but its invalid
            if (field.table == t->name && !t->attributes.contains(field.fieldName)) {
                return {Errors::illegalIdentifier};
            }
            // Find which table we're referring to
            if (t->attributes.contains(field.fieldName)) {
                field.table = t->name;
                return {Errors::noError};
            }
        }
        return {Errors::illegalIdentifier};
    }

    //e.g. what if the user says, "SELECT name, age FROM students WHERE...
  };

}

#endif /* DBQuery_h */


