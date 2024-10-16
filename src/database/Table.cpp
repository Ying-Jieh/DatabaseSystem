//
// Created by Daniel Ziper on 4/19/24.
//

#include "database/Table.hpp"
#include "database/Database.hpp"

namespace ECE141 {

    Table::Table(const std::string &aName, std::shared_ptr<Database> db) : Schema(aName), index(aName), database(db) {
        // Note
    }

    StatusResult Table::addRow(TableRow row, bool shouldValidate) {
        INFO("Table::addRow(TableRow row, bool shouldValidate)")
        StatusResult result = {Errors::noError};
        if (shouldValidate) result = validate(row.getValues());
        if (!result) {return result;};
        resolveAutoIncrement(row);
        IdentifierHash id = row.getIdentifier();
        if (index.contains(id)) {
            ERROR("hashExists, index.contains(id), where id=", id)
            return {Errors::hashExists};
        }
        if (!cache.addRow(id, row, !(bool)database.lock())) {
            ERROR("hashExists, id exists in the cache, where id=", id)
            return {Errors::hashExists};
        }
        size_t startBlock = 0;
        if (auto db = database.lock()) {
            startBlock = db->storage.writeStorable(row);
        }
        index.add(id, startBlock);
        DEBUG("index.add(id, startBlock), where (id, startBlock) = (", id, ", ", startBlock, ")")
        INFO("Successfully added the row (id=", id, ") to ", row.tableName())
        return {Errors::noError};
    };

    void Table::resolveAutoIncrement(ECE141::TableRow &row) {
        if (pkey.has_value() && attributes.get(*pkey)->auto_increment) {
            size_t idx = *attributes.getIndex(*pkey);
            if (std::holds_alternative<AutoIncrementer>(row[idx])) {
                row[idx] = Value((int)index.getNextID());
            }
        }
    }

    StatusResult Table::validate(const std::vector<Value>& values) {
        // Don't check PKey here
        StatusResult error = {Errors::noError};
        std::vector<Value> valsToCheck;
        std::vector<size_t> indicesToCheck;
        attributes.orderedEach([&](size_t idx, const std::string &k, const Attribute &attr) {
            if (attr.unique && !attr.primary_key) {
                if (std::holds_alternative<Null>(values[idx])) {
                    return true;  // Null values don't need to be checked for uniqueness
                }
                valsToCheck.push_back(values[idx]);
                indicesToCheck.push_back(idx);
            }
            return true;
        });
        if (checkValueUnique(indicesToCheck, valsToCheck)) {
            return {Errors::noError};
        } else {
            return {Errors::keyExists};
        }
    }

    bool Table::checkValueUnique(const std::vector<size_t>& idxs, const std::vector<Value>& values) {
        if (idxs.empty()) return true;
        // Loops over each row and checks if the values exist.
        return forEachRow(
                [&](const TableRow& existingRow) {
                    for (size_t i = 0; i < idxs.size(); i++) {
                        if (existingRow[idxs[i]] == values[i]) {
                            return false;
                        }
                    }
                    return true;
                }
            );
    }

    StatusResult Table::addRows(const std::vector<TableRow>& aRows, bool shouldValidate) {
        // Validate all rows, then add together.
        // Add rows one at a time

        return {Errors::notImplemented};
    }

    StatusResult Table::lazyLoadIndex(bool reload) {
        INFO("start lazyLoadIndex")
        if (reload) index.clear(); // TODO: not sure what we want the behavior to be here or when we'd need this
        if (reload || !index.isLoaded()) {
            auto db = database.lock();
            db->storage.readStorable(index, indexLocation);
        }
        INFO("Successfully lazyLoadIndex")
        return {Errors::noError};
    }

    StatusResult Table::forEachRow(TableRowCallback aRowCallback, RequiredCallback aRequirement, IdentifierRequiredCallback aIdentifierRequirement, bool avoidIDNonReqLoads, bool invalidateCache) {
        INFO("start Table::forEachRow")
        auto db = database.lock();
        if (!db) {
            ERROR("unknownDatabase when calling Table::forEachRow, with table: ", name)
            return {Errors::unknownDatabase};
        }
        StatusResult res = lazyLoadIndex();
        if (!res) return res;
        if (invalidateCache) {
            cache.clear();
            DEBUG("The cache is cleared")
        }
        TableRow aRow{this, {}};
        INFO("Calling index.each in the table")
        bool success = index.each([&](IdentifierHash id, uint32_t blocknum){
            TRACE("Got index ", id)
            bool passesIDReq = aIdentifierRequirement(id);
            if (avoidIDNonReqLoads && !passesIDReq) {
                TRACE("Not going to load id ", id)
                return true; // Don't load this row from storage
            }

            // Some duplicate code here, but it avoids unnecessary copies/reads of TableRow
            bool required = false;
            if (!cache.contains(id)) {
                TRACE("The cache does not contain the id: ", id)
                loadRow(aRow, blocknum);
                required = passesIDReq && aRequirement(aRow);  // Note: may short circuit
                cache.addRow(id, aRow, required);
                if (required) return aRowCallback(aRow);
            } else if (passesIDReq) {
                TRACE("The cache contains the id: ", id)
                TableRow& cached = cache.getRow(id);
                if (aRequirement(cached)) {
                    required = true;
                    cache.markRequired(id);
                    return aRowCallback(cached);
                } else {
                    required = false;
                    cache.markRequired(id, false);
                }
            } else {
                cache.markRequired(id, false);
            }
            return true;
        });
        INFO("Finish Table::forEachRow ", success)
        return success ? StatusResult{Errors::noError} : StatusResult{Errors::unknownError};
    }

    StatusResult Table::loadRow(TableRow& aRow, size_t blocknum) {
        database.lock()->storage.readStorable(aRow, blocknum);
        return resolveIntFloat(aRow);
    }

    StatusResult Table::loadAllRows(RequiredCallback aRequirement, IdentifierRequiredCallback aIdentifierRequirement, bool avoidIDNonReqLoads, bool invalidateCache) {
        INFO("Start to load all rows")
        return forEachRow([&](const TableRow& _) {return true;},
                          aRequirement, aIdentifierRequirement, avoidIDNonReqLoads, invalidateCache);
    }

    Table Table::describe() {
        INFO("Start to get information for describing the table: ", name)
        Table tbToShow{"Describe"};
        // Field  | Type  | Null | Key | Default | Extra
        Attribute pk = {"Field", DataTypes::varchar_type};
        pk.primary_key = true;
        tbToShow.addAttribute(pk);
        tbToShow.addAttributes({
                                       {"Type", DataTypes::varchar_type},
                                       {"Null", DataTypes::varchar_type},
                                       {"Key", DataTypes::varchar_type},
                                       {"Default", DataTypes::varchar_type},
                                       {"Extra", DataTypes::varchar_type},
                               });
        attributes.orderedEach([&](size_t index, const std::string& tbName, const Attribute& attr) {
            std::string typeStr{Helpers::dataTypeToString(attr.type)};
            if (attr.field_length) {
                std::stringstream ss;
                ss << "(" << attr.field_length << ")";
                typeStr.append(ss.str());
            }
            std::string isNullStr = attr.nullable ? "Yes" : "No";
            std::string keyStr = attr.unique ? "Yes" : "";
            // Apply the visitor to convert the variant to a string
            std::string defaultStr = Helpers::valueToString(attr.default_value, attr.type);
            std::string extraStr;
            if (attr.auto_increment) { extraStr.append("auto_increment"); }
            if (!extraStr.empty()) extraStr += " ";
            if (attr.primary_key) { extraStr.append("primary key"); }

            TableRow r{&tbToShow, {
                    attr.name, typeStr, isNullStr,
                    keyStr, defaultStr, extraStr
            }};

            tbToShow.addRow(r);
            INFO("Successfully got information for describing the table: ", name)
            return true;
        });
        return tbToShow;
    }

    StatusResult checkValue(Value& value, const Attribute& attr) {
        // Verify type and length, nullable
        if (std::holds_alternative<AutoIncrementer>(value) && !attr.auto_increment) {
            return {Errors::typeMismatch};
        }
        DataTypes valType = Helpers::valueToType(value);
        if (valType == DataTypes::no_type) { // Null
            if (!attr.nullable) {return Errors::notNullable;}
            return {Errors::noError};
        }
        if (valType != attr.type) {
            if (valType == DataTypes::int_type && attr.type == DataTypes::float_type) {
                value = static_cast<double>(std::get<int>(value));
                return {Errors::noError};
            }
            return {Errors::typeMismatch};
        }
        if (valType == DataTypes::varchar_type && attr.field_length != 0) {
            if (std::get<std::string>(value).size() > attr.field_length) {
                return {Errors::lengthMismatch};
            }
        }
        return {Errors::noError};
    }

    StatusResult Table::resolveIntFloat(TableRow& aRow) {
        attributes.orderedEach([&](size_t idx, const std::string& name, const Attribute& attr) {
            if (Helpers::valueToType(aRow[idx]) == DataTypes::int_type && attr.type == DataTypes::float_type) {
                aRow[idx] = static_cast<double>(std::get<int>(aRow[idx]));
            }
            return true;
        });
        return {Errors::noError};
    }

    TableRow Table::buildRow(const std::vector<Value>& values, const TableFieldList& fields, StatusResult& resultOut) {
        INFO("Start to build a row")
        if (values.size() != fields.size() || values.size() > attributes.size()) {
            ERROR("keyValueMismatch when building a row")
            resultOut = {Errors::keyValueMismatch};
            return {};
        }
        TableRow row{this, {}};
        size_t countFound = 0;
        attributes.orderedEach([&](size_t index, const std::string& name, const Attribute& attr) {
            DEBUG("attribute index: ", index, ", attribute name: ", name)
            auto iter = std::find_if(fields.begin(), fields.end(),
                                     [&name](const TableField& field) {
                                                return field.fieldName == name; }
            );
            if (iter == fields.end()) {
                if (std::holds_alternative<Null>(attr.default_value)) {
                    // No default value
                    if (attr.auto_increment) {
                        row.append(AutoIncrementer());
                        TRACE("row.append(AutoIncrementer)")
                        return true;
                    }
                    else if (attr.nullable) {
                        row.append(Null());
                        TRACE("row.append(Null())")
                        return true;
                    } else {
                        ERROR("notNullable")
                        resultOut = {Errors::notNullable, index};
                        return false;
                    }
                } else {
                    row.append(attr.default_value);
                    TRACE("row.append(attr.default_value), where default_value=", Helpers::valueToString(attr.default_value, attr.type))
                    return true;
                }
            }
            size_t valIndex = iter - fields.begin();
            Value val = values[valIndex];
            resultOut = checkValue(val, attr);
            if (!resultOut) return false;

            row.append(val);
//            TRACE("row.append(", Helpers::valueToString(val, attr.type), ")")
            countFound++;
            return true;
        });

        if (!resultOut) return {};
        if (countFound != fields.size()) {
            ERROR("keyValueMismatch: countFound != fields.size(), ", countFound, " != ", fields.size())
            resultOut = {Errors::keyValueMismatch};
            return {};
        }
        INFO("Successfully building the row")
        return row;
    }

    StatusResult Table::close(Database *db) {
        // We're currently in the DB's destructor, so can't use the DB's weak_ptr
        // TODO: check if cache dirty?
        INFO("Start to close a block at index: ", indexLocation)
        if (index.dirty) {
            TRACE("The index, ", indexLocation, " is dirty")
            db->storage.writeStorable(index, indexLocation);
        } else {
            TRACE("The index, ", indexLocation, " is clean")
        }
        INFO("Successfully closing a block")
        return {Errors::noError};
    }

    StatusResult Table::each(ECE141::RowCallback aCallback) {
        // loops over required
        for (const TableRow& row : cache) {
            if (!aCallback(row)) {
                return {Errors::noError};
            }
        }
        return {Errors::noError};
    }

    size_t Table::size() const {
        if (database.lock())
            return index.size();
        return cache.size();
    }

    StatusResult Table::encode(std::ostream& anOutput) const {
        INFO("Start to encode a table: ", name)
        writeBytes(anOutput, indexLocation);
        return Schema::encode(anOutput);
    }

    StatusResult Table::decode(std::istream& anInput) {
        INFO("Start to decode a table: ", name)
        readBytes(anInput, indexLocation);
        return Schema::decode(anInput);
    }
}