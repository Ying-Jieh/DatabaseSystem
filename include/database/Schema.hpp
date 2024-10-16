//
// Created by Daniel Ziper on 4/15/24.
//

#ifndef ECE141DB_SCHEMA_HPP
#define ECE141DB_SCHEMA_HPP

#include <string>
#include <vector>
#include <map>
#include "../misc/BasicTypes.hpp"
#include "misc/Errors.hpp"
#include "misc/Helpers.hpp"
#include "misc/OrderedMap.hpp"
#include "storage/Storage.hpp"

namespace ECE141 {
    constexpr char endOfName='0';

    struct Attribute : public Storable {
        Attribute(std::string aName="", DataTypes aType=DataTypes::varchar_type) : name(aName), type(aType) {};
        ~Attribute() = default;
        Attribute(const Attribute& aCopy) = default;
        Attribute& operator=(const Attribute& aCopy) = default;


        bool operator==(const Attribute& anAttr) const {
            return name == anAttr.name && type == anAttr.type && field_length == anAttr.field_length
                && auto_increment == anAttr.auto_increment && primary_key == anAttr.primary_key
                && nullable == anAttr.nullable
                && default_value == anAttr.default_value;
        }

        std::string name;
        DataTypes type;
        size_t field_length=0;
        bool auto_increment=false;
        bool primary_key=false;
        bool unique=false;
        bool nullable=true;
        Value default_value = Null();

        StatusResult encode(std::ostream& anOutput) const override {
            INFO("Start encoding schema")
            anOutput << name << " ";
            anOutput << (char)type << " ";
            anOutput << field_length << " " << auto_increment << " "  << primary_key << " "
                        << unique << " " << nullable << " ";

            anOutput << Helpers::valueToString(default_value, type) << "\n";
            DEBUG("name=", name, ", type=", (char)type, ", field_length=", field_length, ", auto_increment=", auto_increment,
                  ", primary_key=", primary_key, ", unique=", unique, ", nullable=", nullable)
            INFO("Finish encoding schema")
            return {Errors::noError};
        };

        StatusResult decode(std::istream& anInput) override {
            anInput >> name;
            char typeChar;
            anInput >> typeChar;
            type = DataTypes(typeChar);
            anInput >> field_length >> auto_increment >> primary_key >> unique >> nullable;
            std::string defaultStr;
            anInput >> defaultStr;
            default_value = Helpers::stringToValue(defaultStr, type);

            return {Errors::noError};
        };
        bool initHeader(BlockHeader& header) const override {
            return true; // Not responsible for setting header.
        };
    };

    struct TableRow;

    class Schema : public Storable {
    public:
        explicit Schema(const std::string& aName="") : name(aName) {};
        ~Schema() = default;
        Schema(const Schema& aCopy) {
            *this = aCopy;
        }
        Schema& operator=(const Schema& aCopy);
        bool operator==(const Schema& aSchema) const;

        bool addAttributes(const std::vector<Attribute>& attrs);
        bool addAttribute(const Attribute& anAttr);

        std::optional<std::string> toString(const std::string& key, Value val);
        StatusResult encode(std::ostream& anOutput) const override;
        StatusResult decode(std::istream& anInput) override;
        bool initHeader(BlockHeader &header) const override;
        IdentifierHash getHash() {return Helpers::hash(name);};

        std::string name;
        std::optional<std::string> pkey;
        IndexedMap<std::string, Attribute> attributes;
    };

}

#endif //ECE141DB_SCHEMA_HPP
