//
// Created by Daniel Ziper on 5/23/24.
//

#ifndef ECE141DB_JOINEDROW_HPP
#define ECE141DB_JOINEDROW_HPP

#include "../include/database/TableRow.hpp"
#include "../include/database/Table.hpp"

namespace ECE141 {
    struct JoinedRow : public Row {
        JoinedRow(const std::vector<const TableRow*> aRows, const TableFieldList& aOutputFields) :
           outputFields(aOutputFields) {
            setRows(aRows);
        }
        ~JoinedRow() = default;

        void setRows(const std::vector<const TableRow*>& aRows) {
            size_t idx = 0;
            for (auto row : aRows) {
                rows[idx] = row;
                idx++;
            }
        }

        std::optional<Value> getItem(const std::string& aName) const override {
            for (auto r: rows) {
                if (!r) break;
                if (auto idx = r->getSchema()->attributes.getIndex(aName)) {
                    return r->operator[](*idx);
                }
            }
            return std::nullopt;
        }
        std::optional<Value> getItem(const TableField& aField) const override {
            const TableRow* r = findRowForTable(aField.table);
            if (!r) return std::nullopt;
            return r->getItem(aField.fieldName);
        }
        Value operator[](size_t index) const override {
            return *getItem(outputFields[index]);
        };

        std::optional<std::string> getString(const std::string& aName) const override {
            for (auto r: rows) {
                if (r->getSchema()->attributes.contains(aName)) {
                    return r->getString(aName);
                }
            }
            return std::nullopt;
        }
        std::optional<std::string> getString(const TableField& aField) const override {
            const TableRow* r = findRowForTable(aField.table);
            if (!r) return std::nullopt;
            return r->getString(aField);
        }

        const std::vector<Value>& getValues() const override {
            if (!valueCached.empty()) return valueCached;
            for (auto r : rows) {
                if (!r) break;
                for (auto& v : r->getValues()) {
                    valueCached.emplace_back(v);
                }
            }
            return valueCached;
        }

    protected:
        const TableRow* rows[Config::joinedRowSize]; // TODO: Consider changing this to SharedTableRow - the pointer MAY move if RowCollection/RowCache is modified
        mutable std::vector<Value> valueCached;
        const TableFieldList& outputFields;

        const TableRow* findRowForTable(const std::string& name) const {
            for (auto r : rows) {
                if (!r) break;
                if (r->tableName() == name) return r;
            }
            return nullptr;
        }
    };

    struct NullRow : public TableRow {
        // Acts as placeholder for a Row object when none is found and is necessary by the join
        NullRow() {}
        NullRow(Schema* schema) : TableRow(schema, {}) {};
        std::optional<Value> getItem(const std::string &aName) const override {
            return {Null()};
        }
        std::optional<Value> getItem(const TableField &aName) const override {
            return {Null()};
        }
        Value operator[](size_t index) const override {
            return {Null()};
        }
        std::optional<std::string> getString(const std::string &aName) const override {
            return {Null::toString()};
        }
        std::optional<std::string> getString(const TableField &aName) const override {
            return {Null::toString()};
        }
        const std::vector<Value> &getValues() const override {
            if (!nulls.empty()) return nulls;
            nulls.reserve(size);
            for (size_t idx = 0; idx < size; idx++) {
                nulls.push_back(Null());
            }
            return nulls;
        }
    protected:
        size_t size;
        mutable std::vector<Value> nulls;
    };
}

#endif //ECE141DB_JOINEDROW_HPP
