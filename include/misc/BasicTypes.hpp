//
//  BasicTypes.hpp
//  RGAssignement1
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef BasicTypes_h
#define BasicTypes_h

#include <optional>
#include <variant>
#include <vector>
#include <string>

namespace ECE141 {

    enum class DataTypes {
        no_type='N',  bool_type='B', datetime_type='D',
         float_type='F', int_type='I', varchar_type='V',
     };

    struct TableName {
        TableName(const std::string &aTableName="", const std::string &anAlias="")
                : table(aTableName), alias(anAlias) {}
        TableName(const TableName &aCopy) : table(aCopy.table), alias(aCopy.alias) {}
        TableName& operator=(const std::string &aName) {
            table=aName;
            return *this;
        }
        bool operator==(const TableName& other) const {
            return table == other.table && alias == other.alias;
        }
        operator const std::string() {return table;}
        std::string table;
        std::string alias;
    };

    struct TableField {
        TableField(const std::string &aName="", const std::string &aTable="", const std::string &anAlias="")
                : fieldName(aName), table(aTable) {
            alias = anAlias.empty() ? aName : anAlias;
        }

        TableField(const TableField &aCopy) : fieldName(aCopy.fieldName), table(aCopy.table), alias(aCopy.alias) {}
        bool operator==(const TableField& aTableField) const {
            return this->fieldName == aTableField.fieldName && this->table == aTableField.table && this->alias == aTableField.alias;
        }
        std::string fieldName;
        std::string table;
        std::string alias;
    };

    struct OrderByField {
        TableField field;
        bool ascending;
    };

    using OptString = std::optional<std::string_view>;
    using StringList = std::vector<std::string>;
    using TableFieldList = std::vector<TableField>;

    struct Null {
        static std::string toString() {
            return "NULL";
        }
        bool operator==(const Null& aNull) const {return true;}
        bool operator>(const Null& aNull) const {return false;}
        bool operator<(const Null& aNull) const {return false;}
    };
    using OptString = std::optional<std::string_view>;

    struct AutoIncrementer {
        static std::string toString() {
            return "AUTOINCREMENTER PLACEHOLDER";
        }
        bool operator==(const AutoIncrementer& other) const {return true;}
        bool operator>(const AutoIncrementer& other) const {return false;}
        bool operator<(const AutoIncrementer& other) const {return false;}
    };

    using IdentifierHash = uint64_t;
    using Value = std::variant<Null, AutoIncrementer, bool, int, double, std::string>;

    template <typename CRTP>
    class AsSingleton {
    public:
        static CRTP& getInstance() {
            static CRTP theInstance;
            return theInstance;
        }
    };

}
#endif /* BasicTypes_h */
