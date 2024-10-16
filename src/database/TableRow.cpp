//
// Created by Daniel Ziper on 4/19/24.
//

#include "database/TableRow.hpp"
#include "database/Table.hpp"
#include "tokenizer/Tokenizer.hpp"
#include "parsing/TokenParser.hpp"
#include "parsing/MultiParser.hpp"

namespace ECE141 {
    TableRow::TableRow(Schema* aSchema, const std::vector<Value>& aValues) {
        schema = aSchema;
        values = aValues;
    };

    TableRow::TableRow(const TableRow& aCopy) {
        schema = aCopy.schema;
        identifier = aCopy.identifier;
        for (const auto& v : aCopy.values) {
            values.push_back(v);
        }
    }

    std::optional<bool> TableRow::updateKey(const std::string& aKey, Value aValue) {
        INFO("TableRow::updateKey(", aKey, ", aValue)")
        if (auto index = schema->attributes.getIndex(aKey)) {
            if (values[*index] == aValue) {
                DEBUG("values[*index] == aValue, return false")
                return false;
            }
            values[*index] = aValue;
            DEBUG("values[*index] = aValue, where index=", *index)
            if (!schema->pkey.has_value() || aKey == *schema->pkey) identifier = std::nullopt; // Need to (lazily) recompute ID
            DEBUG("return true")
            return true;
        }
        INFO("return std::nullopt")
        return std::nullopt;
    }

    IdentifierHash TableRow::getIdentifier() const {
        // Lazily compute identifier
        INFO("TableRow::getIdentifier()")
        if (identifier.has_value()) {
            DEBUG("identifier.has_value()")
            DEBUG("return *identifier, where *identifier=", *identifier)
            return *identifier;
        }
        if (schema->pkey.has_value()) {
            DEBUG("schema->pkey.has_Value()")
            std::string pkey = *schema->pkey;
            DEBUG("pkey = ", pkey)
            if (schema->attributes.get(pkey)->type == DataTypes::int_type) {
                DEBUG("schema->attributes.get(pkey)->type == DataTypes::int_type")
                identifier = std::get<int>(*getItem(pkey));
                // Will throw if AutoIncrement not resolved, shouldn't ever happen
            } else {
                DEBUG("schema->attributes.get(pkey)->type != DataTypes::int_type")
                // Hash the string representation
                identifier = Helpers::hash(*getString(pkey));
            }
        } else {
            DEBUG("!schema->pkey.has_Value()")
            // Hash the entire row, there's no pkey
            std::stringstream ss;
            encode(ss);
            identifier = Helpers::hash(ss.str());
        }
        DEBUG("return *identifier, where *identifier=", *identifier)
        return *identifier;
    }

    std::optional<Value> TableRow::getItem(const std::string& aName) const {
        INFO("TableRow::GetItem(", aName, ")")
        if (auto index = schema->attributes.getIndex(aName)) {
            DEBUG("return values[*index], where *index=", *index)
            return values[*index];
        }
        DEBUG("return std::nullopt")
        return std::nullopt;
    }

    std::optional<Value> TableRow::getItem(const TableField& aField) const {
        INFO("TableRow::getItem(aField)")
        DEBUG("where fieldName=", aField.fieldName)
        if (aField.table.empty() || aField.table == schema->name) {
            DEBUG("aField.table.empty() || aField.table == schema->name, return getItem(aField.fieldName), where fieldName=", aField.fieldName)
            return getItem(aField.fieldName);
        }
        else {
            DEBUG("return std::nullopt")
            return std::nullopt;
        }
    }

    Value TableRow::operator[](size_t index) const {
        return values[index];
    }

    Value& TableRow::operator[](size_t index) {
        return values[index];
    }

    std::optional<std::string> TableRow::getString(const TableField& aField) const {
        if (aField.table == schema->name) return getString(aField.fieldName);
        else return std::nullopt;
    }

    std::optional<std::string> TableRow::getString(const std::string& aName) const {
        if (auto item = getItem(aName)) {
            return schema->toString(aName, *item);
        }
        return std::nullopt;
    }

    StatusResult TableRow::encode(std::ostream& anOutput) const {
        INFO("TableRow::encode(std::ostream& anOutput)")
        size_t count = 0;
        if (!schema) {
            ERROR("unknownTable")
            return {Errors::unknownTable};
        }
        bool success = schema->attributes.orderedEach([&](size_t index, const std::string& key, const Attribute& attr) {
            if (index >= values.size()) {
                return false;
            }
            std::string valString = Helpers::valueToParseableString(values[index], attr.type); //TODO: bug for auto_increment?
            anOutput << valString;
            if (index != values.size() - 1)
                anOutput << ",";
            count++;
            return true;
        });
        if (success || count != values.size()) return {Errors::noError};
        else {
            ERROR("keyValueMismatch")
            return {Errors::keyValueMismatch};
        }
    };
    StatusResult TableRow::decode(std::istream& anInput) {
        INFO("TableRow::decode(std::istream& anInput)")
        if (!schema) {
            ERROR("unknownTable")
            return {Errors::unknownTable};
        }
        Tokenizer theTokenizer(anInput);
        StatusResult theResult = theTokenizer.tokenize();
        TokenParser tokenParser(theTokenizer);
        ValueParser vp;
        MultiParser<Value> mp{vp, ",", "", ""};
        theResult = tokenParser.parseWith(mp).getState();
        values = mp.out;
        identifier = std::nullopt;
        DEBUG("return theResult: ", theResult)
        return theResult;
    }
    bool TableRow::initHeader(BlockHeader& header) const {
        INFO("TableRow::initHeader(BlockHeader& header)")
        header.type = BlockType::data;
        if (schema) {
            DEBUG("header.hash = schema->getHash(), where schema->getHash()=", schema->getHash())
            header.hash = schema->getHash();
        }
        return true;
    };
}