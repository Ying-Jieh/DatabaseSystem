//
// Created by Daniel Ziper on 4/15/24.
//

#ifndef ECE141DB_TABLE_HPP
#define ECE141DB_TABLE_HPP
#include "misc/Errors.hpp"
#include "Schema.hpp"
#include "Index.hpp"
#include "TableRow.hpp"
#include "Database.hpp"
#include "RowCache.hpp"
#include "RowProvider.hpp"

namespace ECE141 {
    class Database;
    class RowCache;

    class Table : public Schema, ByteIO<uint32_t>, public RowProvider {
    public:
        Table(const std::string& aName="", std::shared_ptr<Database> db = nullptr);
        Table(const Table& aCopy) {
            *this = aCopy;
        }

        Table& operator=(const Table& aCopy) {
            if (this == &aCopy) return *this;
            Schema::operator=(aCopy);
            database = aCopy.database;
            cache = aCopy.cache;
            index = aCopy.index;
            indexLocation = aCopy.indexLocation;
            return *this;
        }

        StatusResult addRow(TableRow row, bool shouldValidate=true);
        StatusResult addRows(const std::vector<TableRow>& aRows, bool shouldValidate=true);
        void resolveAutoIncrement(TableRow& row);

        using IdentifierRequiredCallback = std::function<bool(IdentifierHash)>;
        using RequiredCallback = std::function<bool(const TableRow&)>;
        using TableRowCallback = std::function<bool(const TableRow&)>;

        StatusResult lazyLoadIndex(bool reload = false);
        StatusResult loadAllRows(
                RequiredCallback aRequirement=[](const TableRow&){return true;}, // Returns true if this row should be marked required in Cache
                IdentifierRequiredCallback aIdentifierRequirement=[](IdentifierHash){return true;}, // Return true if a row with this ID should be marked required in the Cache
                bool avoidIDNonReqLoads=false, // Don't load from Storage (or load into cache) if fails ID requirement
                bool invalidateCache=false // Clear the cache before loading anything
        );
        StatusResult forEachRow(
                TableRowCallback aRowCallback,
                RequiredCallback aRequirement=[](const TableRow&){return true;}, // Returns true if this row should be marked required in Cache
                IdentifierRequiredCallback aIdentifierRequirement=[](IdentifierHash){return true;}, // Return true if a row with this ID should be marked required in the Cache
                bool avoidIDNonReqLoads=false, // Don't load from Storage (or load into cache) if fails ID requirement
                bool invalidateCache=false
        );
        StatusResult each(RowCallback aCallback) override;

        size_t size() const override;
        Table describe();
        StatusResult close(Database* db);

        TableRow buildRow(const std::vector<Value>& values, const TableFieldList& fields, StatusResult& resultOut);
        StatusResult validate(const std::vector<Value>& values);

        // Note: Only encodes/decodes the Schema definition and the indexLocation
        // Use index.encode/decode (at indexLocation) to read the index from storage
        StatusResult encode(std::ostream& anOutput) const override;
        StatusResult decode(std::istream& anInput) override;

        RowCache cache;
        Index<uint32_t> index;
        uint32_t indexLocation;
    protected:
        std::weak_ptr<Database> database;
        bool checkValueUnique(const std::vector<size_t>& idxs, const std::vector<Value>& values);
        StatusResult loadRow(TableRow& aRow, size_t blocknum);
        StatusResult resolveIntFloat(TableRow& idx);
    };
}

#endif //ECE141DB_TABLE_HPP
