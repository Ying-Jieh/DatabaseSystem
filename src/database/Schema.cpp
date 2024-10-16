//
//  Schema.cpp
//  PA3
//
//  Created by rick gessner on 3/2/23.
//
//
//#include "database/Schema.hpp"
//
//namespace ECE141 {
//
// //STUDENT: Implement this class...
//
//  Schema::Schema(const std::string aName) : name(aName){}
//
//  Schema::Schema(const Schema &aCopy) {
//    name=aCopy.name;
//  }
//
//  Schema::~Schema() {
//    //std::cout << "~Schema()\n";
//  }
//
//
//}

#include "database/TableRow.hpp"
#include "database/Schema.hpp"

namespace ECE141 {

    Schema& Schema::operator=(const Schema &aCopy) {
        name = aCopy.name;
        pkey = aCopy.pkey;
        aCopy.attributes.orderedEach([&](size_t idx, const std::string &k, const Attribute &attr) {
            attributes.insert(k, attr);
            return true;
        });
        return *this;
    }

    bool Schema::operator==(const Schema &aSchema) const {
        return name == aSchema.name && attributes == aSchema.attributes;
    }

    bool Schema::addAttributes(const std::vector<Attribute> &attrs) {
        INFO("Start to add the attributes to the schema")
        for (auto &attr: attrs) {
            addAttribute(attr);
            DEBUG("Added attribute ", attr.name)
        }
        INFO("Successfully added the attributes to the schema")
        return true;
    }

    bool Schema::addAttribute(const Attribute &anAttr) {
        if (attributes.contains(anAttr.name)) {
            WARN(anAttr.name, " already in the attributes")
            return false;
        }
        if (anAttr.primary_key) {
            if (pkey.has_value()) {
                ERROR("The primary key, ", anAttr.name, " has no value")
                return false;
            }
            pkey = anAttr.name;
        }
        attributes.insert(anAttr.name, anAttr);
        DEBUG("attributes.insert(anAttr.name, anAttr), where anAttr.name=", anAttr.name)
        return true;
    }

    std::optional<std::string> Schema::toString(const std::string &key, Value val) {
        TRACE("Start to convert the attributes to strings")
        if (auto _ = std::get_if<Null>(&val)) {
            return Null::toString();
        }
        if (auto attr = attributes.get(key)) {
            return Helpers::valueToString(val, attr->type);
        }
        WARN("Failed to convert the attribute to string, ", key)
        return std::nullopt;
    }

    StatusResult Schema::encode(std::ostream &anOutput) const {
        INFO("Start to encode the schema")
        StatusResult theResult;
        anOutput << name << " ";
        TRACE("Encoded name ", name)
        attributes.orderedEach([&](size_t index, const std::string &key, const Attribute &value) {
            theResult = value.encode(anOutput);
            DEBUG("Encoded attribute ", value.name)
//TODO: handle errors
            return true;
        });
        INFO("Successfully encoded the schema")
        return {Errors::noError};
    }

    StatusResult Schema::decode(std::istream &anInput) {
        INFO("Start to decode the schema")
        anInput >> name;
        TRACE("Decoded name ", name)
        attributes.clear();
        Attribute theAttribute;
        while (!anInput.eof() && theAttribute.decode(anInput)) {
            addAttribute(theAttribute);
            DEBUG("Decoded attribute ", theAttribute.name)
        }
        INFO("Successfully encoded the schema")
        return {Errors::noError};
    }

    bool Schema::initHeader(BlockHeader &header) const {
        header.type = BlockType::schema;
        INFO("Initialized the block header for schema")
        return true;
    }

}

