//
// Created by Daniel Ziper on 4/15/24.
//

#ifndef ECE141DB_TABLEROW_HPP
#define ECE141DB_TABLEROW_HPP

#include <variant>
#include <string>
#include <map>
#include <optional>
#include <memory>
#include "misc/BasicTypes.hpp"
#include "storage/Storage.hpp"
#include "Schema.hpp"

namespace ECE141 {
    struct Row {
        virtual std::optional<Value> getItem(const std::string& aName) const = 0;
        virtual std::optional<Value> getItem(const TableField& aName) const = 0;
        virtual Value operator[](size_t index) const = 0;
        virtual std::optional<std::string> getString(const std::string& aName) const = 0;
        virtual std::optional<std::string> getString(const TableField& aName) const = 0;
        virtual const std::vector<Value>& getValues() const = 0;
    };

    struct TableRow : public Storable, public Row {
    protected:
        Schema* schema;
        StatusResult error;
        mutable std::optional<IdentifierHash> identifier; // Mutable because lazily computed
        std::vector<Value> values;
    public:
        TableRow() = default;
        ~TableRow() = default;
        TableRow(const TableRow& aCopy);
        TableRow(Schema* aSchema, const std::vector<Value>& values);

        IdentifierHash getIdentifier() const;
        std::optional<Value> getItem(const std::string& aName) const override;
        std::optional<Value> getItem(const TableField& aName) const override;
        Value& operator[](size_t index);
        Value operator[](size_t index) const override;
        std::optional<std::string> getString(const std::string& aName) const override;
        std::optional<std::string> getString(const TableField& aName) const override;
        std::optional<bool> updateKey(const std::string& aName, Value aValue);

        StatusResult encode(std::ostream& anOutput) const override;
        StatusResult decode(std::istream& anInput) override;
        bool initHeader(BlockHeader& header) const override;
        void append(const Value& val) {values.emplace_back(val);}
        size_t size() {return values.size();}
        const std::string& tableName() const {return schema->name;}
        const Schema* getSchema() const {return schema;}

        const std::vector<Value>& getValues() const override {return values;};
    };

    using SharedRow = std::shared_ptr<Row>;
    using SharedTableRow = std::shared_ptr<TableRow>;
}

#endif //ECE141DB_TABLEROW_HPP
