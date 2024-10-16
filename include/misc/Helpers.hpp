//
//  Helpers.hpp
//  Database3
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef Helpers_h
#define Helpers_h

#include "BasicTypes.hpp"
#include "tokenizer/keywords.hpp"
#include "Errors.hpp"
#include <algorithm>

namespace ECE141 {

    static std::map<std::string, Operators> gExpressionOps = {
            {"=",   Operators::equal_op},
            {"<",   Operators::lt_op},
            {"<=",  Operators::lte_op},
            {">",   Operators::gt_op},
            {">=",  Operators::gte_op},
            {"!=",  Operators::notequal_op},
            {"not", Operators::notequal_op},
    };

    static std::map<std::string, Operators> gOperators = {
            std::make_pair(".", Operators::dot_op),
            std::make_pair("+", Operators::add_op),
            std::make_pair("-", Operators::subtract_op),
            std::make_pair("*", Operators::multiply_op),
            std::make_pair("/", Operators::divide_op),
            std::make_pair("^", Operators::power_op),
            std::make_pair("%", Operators::mod_op),
            std::make_pair("=", Operators::equal_op),
            std::make_pair("!=", Operators::notequal_op),
            std::make_pair("not", Operators::notequal_op),
            std::make_pair("<", Operators::lt_op),
            std::make_pair("<=", Operators::lte_op),
            std::make_pair(">", Operators::gt_op),
            std::make_pair(">=", Operators::gte_op),
            std::make_pair("and", Operators::and_op),
            std::make_pair("or", Operators::or_op),
            std::make_pair("nor", Operators::nor_op),
            std::make_pair("between", Operators::between_op),
    };

//  static ECE141::Keywords gJoinTypes[]={
//    ECE141::Keywords::cross_kw,ECE141::Keywords::full_kw, ECE141::Keywords::inner_kw,
//    ECE141::Keywords::left_kw, ECE141::Keywords::right_kw
//  };

    //a list of known functions...
    static std::map<std::string,int> gFunctions = {
            std::make_pair("avg", 10),
            std::make_pair("count", 20),
            std::make_pair("max", 30),
            std::make_pair("min", 40)
    };


    //This map binds a keyword string with a Keyword (token)...
    static std::map<std::string,  Keywords> gDictionary = {
            std::make_pair("about",     Keywords::about_kw),
            std::make_pair("add",       Keywords::add_kw),
            std::make_pair("all",       Keywords::all_kw),
            std::make_pair("alter",     Keywords::alter_kw),
            std::make_pair("and",       Keywords::and_kw),
            std::make_pair("as",        Keywords::as_kw),
            std::make_pair("asc",       Keywords::asc_kw),
            std::make_pair("avg",       ECE141::Keywords::avg_kw),
            std::make_pair("auto_increment", Keywords::auto_increment_kw),
            std::make_pair("between",   ECE141::Keywords::between_kw),
            std::make_pair("boolean",   ECE141::Keywords::boolean_kw),
            std::make_pair("by",        ECE141::Keywords::by_kw),
            std::make_pair("change",    ECE141::Keywords::change_kw),
            std::make_pair("changed",    ECE141::Keywords::changed_kw),
            std::make_pair("char",      ECE141::Keywords::char_kw),
            std::make_pair("column",    ECE141::Keywords::column_kw),
            std::make_pair("count",     ECE141::Keywords::count_kw),
            std::make_pair("create",    ECE141::Keywords::create_kw),
            std::make_pair("cross",     ECE141::Keywords::cross_kw),
            std::make_pair("current_date",  Keywords::current_date_kw),
            std::make_pair("current_time",  Keywords::current_time_kw),
            std::make_pair("current_timestamp", Keywords::current_timestamp_kw),
            std::make_pair("database",  ECE141::Keywords::database_kw),
            std::make_pair("databases", ECE141::Keywords::databases_kw),
            std::make_pair("datetime",  ECE141::Keywords::datetime_kw),
            std::make_pair("decimal",   ECE141::Keywords::decimal_kw),
            std::make_pair("desc",      ECE141::Keywords::desc_kw),
            std::make_pair("default",    ECE141::Keywords::default_kw),
            std::make_pair("delete",    ECE141::Keywords::delete_kw),
            std::make_pair("describe",  ECE141::Keywords::describe_kw),
            std::make_pair("distinct",  ECE141::Keywords::distinct_kw),
            std::make_pair("double",    ECE141::Keywords::double_kw),
            std::make_pair("drop",      ECE141::Keywords::drop_kw),
            std::make_pair("dump",      ECE141::Keywords::dump_kw),
            std::make_pair("enum",      ECE141::Keywords::enum_kw),
            std::make_pair("error",     ECE141::Keywords::error_kw),
            std::make_pair("explain",   ECE141::Keywords::explain_kw),
            std::make_pair("false",     ECE141::Keywords::false_kw),
            std::make_pair("float",     ECE141::Keywords::float_kw),
            std::make_pair("foreign",   ECE141::Keywords::foreign_kw),
            std::make_pair("from",      ECE141::Keywords::from_kw),
            std::make_pair("full",      ECE141::Keywords::full_kw),
            std::make_pair("group",     ECE141::Keywords::group_kw),
            std::make_pair("help",      ECE141::Keywords::help_kw),
            std::make_pair("in",        ECE141::Keywords::in_kw),
            std::make_pair("index",     ECE141::Keywords::index_kw),
            std::make_pair("indexes",   ECE141::Keywords::indexes_kw),
            std::make_pair("inner",     ECE141::Keywords::inner_kw),
            std::make_pair("insert",    ECE141::Keywords::insert_kw),
            std::make_pair("int",       ECE141::Keywords::integer_kw),
            std::make_pair("integer",   ECE141::Keywords::integer_kw),
            std::make_pair("into",      ECE141::Keywords::into_kw),
            std::make_pair("join",      ECE141::Keywords::join_kw),
            std::make_pair("key",       ECE141::Keywords::key_kw),
            std::make_pair("last",      ECE141::Keywords::last_kw),
            std::make_pair("left",      ECE141::Keywords::left_kw),
            std::make_pair("like",      ECE141::Keywords::like_kw),
            std::make_pair("limit",     ECE141::Keywords::limit_kw),
            std::make_pair("max",       ECE141::Keywords::max_kw),
            std::make_pair("min",       ECE141::Keywords::min_kw),
            std::make_pair("modify",    ECE141::Keywords::modify_kw),
            std::make_pair("not",       ECE141::Keywords::not_kw),
            std::make_pair("null",      ECE141::Keywords::null_kw),
            std::make_pair("on",        ECE141::Keywords::on_kw),
            std::make_pair("or",        ECE141::Keywords::or_kw),
            std::make_pair("order",     ECE141::Keywords::order_kw),
            std::make_pair("outer",     ECE141::Keywords::outer_kw),
            std::make_pair("primary",   ECE141::Keywords::primary_kw),
            std::make_pair("query",     ECE141::Keywords::query_kw),
            std::make_pair("quit",      ECE141::Keywords::quit_kw),
            std::make_pair("references",ECE141::Keywords::references_kw),
            std::make_pair("right",     ECE141::Keywords::right_kw),
            std::make_pair("row",       ECE141::Keywords::row_kw),
            std::make_pair("rows",      ECE141::Keywords::rows_kw),
            std::make_pair("run",       ECE141::Keywords::run_kw),
            std::make_pair("select",    ECE141::Keywords::select_kw),
            std::make_pair("self",      ECE141::Keywords::self_kw),
            std::make_pair("set",       ECE141::Keywords::set_kw),
            std::make_pair("show",      ECE141::Keywords::show_kw),
            std::make_pair("sum",       ECE141::Keywords::sum_kw),
            std::make_pair("table",     ECE141::Keywords::table_kw),
            std::make_pair("tables",    ECE141::Keywords::tables_kw),
            std::make_pair("timestamp", ECE141::Keywords::timestamp_kw),
            std::make_pair("true",      ECE141::Keywords::true_kw),
            std::make_pair("unique",    ECE141::Keywords::unique_kw),
            std::make_pair("update",    ECE141::Keywords::update_kw),
            std::make_pair("use",       ECE141::Keywords::use_kw),
            std::make_pair("values",    ECE141::Keywords::values_kw),
            std::make_pair("varchar",   ECE141::Keywords::varchar_kw),
            std::make_pair("version",   ECE141::Keywords::version_kw),
            std::make_pair("where",     ECE141::Keywords::where_kw)
    };

    class Helpers {
    public:

        static Keywords getKeywordId(const std::string aKeyword) {
            auto theIter = gDictionary.find(aKeyword);
            if (theIter != gDictionary.end()) {
                return theIter->second;
            }
            return Keywords::unknown_kw;
        }

        //convert from char to keyword...
        static Keywords charToKeyword(char aChar) {
            switch(toupper(aChar)) {
                case 'I': return Keywords::integer_kw;
                case 'T': return Keywords::datetime_kw;
                case 'B': return Keywords::boolean_kw;
                case 'F': return Keywords::float_kw;
                case 'V': return Keywords::varchar_kw;
                default:  return Keywords::unknown_kw;
            }
        }

        static Logical KeywordToLogical(Keywords aKeyword) {
            switch(aKeyword) {
                case Keywords::and_kw: return Logical::and_op;
                case Keywords::or_kw: return Logical::or_op;
                case Keywords::not_kw: return Logical::not_op;
                default:  return Logical::unknown_op;
            }
        }

        static const char* dataTypeToString(DataTypes aType) {
            switch(aType) {
                case DataTypes::bool_type:      return "bool";
                case DataTypes::datetime_type:  return "datetime";
                case DataTypes::float_type:     return "float";
                case DataTypes::int_type:       return "int";
                case DataTypes::varchar_type:   return "varchar";
                default:   return "no type";
            }
        }

        static const char* keywordToString(Keywords aType) {
            switch(aType) {
                case Keywords::boolean_kw:    return "bool";
                case Keywords::create_kw:     return "create";
                case Keywords::database_kw:   return "database";
                case Keywords::databases_kw:  return "databases";
                case Keywords::datetime_kw:   return "datetime";
                case Keywords::describe_kw:   return "describe";
                case Keywords::drop_kw:       return "drop";
                case Keywords::float_kw:      return "float";
                case Keywords::integer_kw:    return "integer";
                case Keywords::show_kw:       return "show";
                case Keywords::table_kw:      return "table";
                case Keywords::tables_kw:     return "tables";
                case Keywords::use_kw:        return "use";
                case Keywords::varchar_kw:    return "varchar";
                default:                      return "unknown";
            }
        }

        static const DataTypes keywordToDataType(Keywords aType) {
            switch(aType) {
                case Keywords::boolean_kw:    return DataTypes::bool_type;
                case Keywords::datetime_kw:   return DataTypes::datetime_type;
                case Keywords::float_kw:
                case Keywords::timestamp_kw:  return DataTypes::float_type; // TODO: is this right?
                case Keywords::integer_kw:    return DataTypes::int_type;
                case Keywords::varchar_kw:    return DataTypes::varchar_type;
                default:                      return DataTypes::no_type;
            }
        }

        static const char* ErrorToString(Errors aType) {
            switch(aType) {
                case Errors::noError:      return "No Error";
                case Errors::userTerminated:      return "User Terminated";
                case Errors::illegalIdentifier:   return "Illegal identifier";
                case Errors::identifierExpected:  return "Identifier Expected";
                case Errors::unknownIdentifier:   return "Unknown identifier";
                case Errors::databaseExists:      return "Database exists";
                case Errors::tableExists:         return "Table Exists";
                case Errors::syntaxError:         return "Syntax Error";
                case Errors::unknownCommand:      return "Unknown command";
                case Errors::unknownDatabase:     return "Unknown database";
                case Errors::unknownTable:        return "Unknown table";
                case Errors::noDatabaseSpecified: return "No database specified";
                case Errors::punctuationExpected: return "Punctuation Expected";
                case Errors::unexpectedKeyword:   return "Unexpected Keyword";
                case Errors::valueExpected:       return "Value Expected";
                case Errors::keyValueMismatch:    return "Key-Value Mismatch";
                case Errors::parsingError:        return "Parsing Error";
                default:                          return "Unknown error";
            }
        };

        // USE: ---validate that given keyword is a datatype...
        static bool isDatatype(Keywords aKeyword) {
            switch(aKeyword) {
                case Keywords::char_kw:
                case Keywords::datetime_kw:
                case Keywords::float_kw:
                case Keywords::integer_kw:
                case Keywords::varchar_kw:
                    return true;
                default: return false;
            }
        }

        static Operators toOperator(std::string aString) {
            auto theIter = gOperators.find(aString);
            if (theIter != gOperators.end()) {
                return theIter->second;
            }
            return Operators::unknown_op;
        }

        static int getFunctionId(const std::string anIdentifier) {
            auto theIter = gFunctions.find(anIdentifier);
            if (theIter != gFunctions.end()) {
                return theIter->second;
            }
            return 0;
        }

        static bool isNumericKeyword(Keywords aKeyword) {
            static Keywords theTypes[]={Keywords::decimal_kw, Keywords::double_kw, Keywords::float_kw, Keywords::integer_kw};
            for(auto k : theTypes) {
                if(aKeyword==k) return true;
            }
            return false;
        }

        static std::string doubleToString(double value) {
            std::string str = std::to_string(value);
            size_t dotPos = str.find('.');
            if (dotPos != std::string::npos) {
                size_t lastNonZeroPos = str.find_last_not_of('0');
                if (lastNonZeroPos == dotPos) // If only zeros after decimal point, remove it too
                    str.erase(lastNonZeroPos);
                else if (lastNonZeroPos != std::string::npos)
                    str.erase(lastNonZeroPos + 1); // Erase the trailing zeros
            }
            return str;
        }

        static bool isInt(const std::string& aStr) {
            try {
                size_t pos = 0;
                std::stoi(aStr, &pos);
                return pos == aStr.size();
            } catch (...) {
                return false;
            }
        }

        static DataTypes valueToType(const Value& val) {
            switch (val.index()) {
                case 0: return DataTypes::no_type;
                case 1: return DataTypes::int_type;  // Autoincrementer is Int
                case 2: return DataTypes::bool_type;
                case 3: return DataTypes::int_type;
                case 4: return DataTypes:: float_type;
                case 5: return DataTypes::varchar_type;
                default: return DataTypes::no_type;
            }
        }

        static std::string valueToString(const Value& val, DataTypes type) {
            if (auto _ = std::get_if<Null>(&val)) {
                return Null::toString();
            }
            switch (type) {
                case DataTypes::no_type: return "";
                case DataTypes::varchar_type: return std::get<std::string>(val);

                case DataTypes::bool_type: return std::get<bool>(val) ? "True" : "False";
                case DataTypes::datetime_type: return "NOT IMPLEMENTED";

                case DataTypes::float_type: return Helpers::doubleToString(std::get<double>(val));

                case DataTypes::int_type: return std::to_string(std::get<int>(val)); //TODO: auto_increment now has no value
            }
            return "";
        }

        static std::string valueToParseableString(Value val, DataTypes type) {
            if (auto _ = std::get_if<Null>(&val)) {
                return Null::toString();
            }
            if (auto _ = std::get_if<AutoIncrementer>(&val)) {
                return AutoIncrementer::toString();
            }
            if (type == DataTypes::varchar_type) {
                return "\'" + std::get<std::string>(val) + "\'";
            }
            return valueToString(val, type);
        }

        static IdentifierHash hash(std::string str) {
            static std::hash<std::string> StringHasher;
            return StringHasher(str);
        }

        static Value stringToValue(const std::string& aString, DataTypes type) {
            if (aString == "NULL") {
                Null null;
                return null;
            }
            switch (type) {
                case DataTypes::no_type: return "";
                case DataTypes::varchar_type: return aString;

                case DataTypes::bool_type: {
                    if (aString == "true" || aString == "True") return true;
                    return false;
                }
                case DataTypes::datetime_type: return "NOT IMPLEMENTED";

                case DataTypes::float_type: return std::stod(aString);

                case DataTypes::int_type: return std::stoi(aString);
            }
            return "";
        }

        template<typename T>
        static bool in_array(const std::vector<T>& aVector, T t) {
            if (std::find(aVector.begin(), aVector.end(), t) != aVector.end()) {
                return true;
            }
            return false;
        }

        static Operators inverseOp(Operators& anOp) {
            switch (anOp) {
                case Operators::equal_op:    return Operators::notequal_op;
                case Operators::notequal_op: return Operators::equal_op;
                case Operators::lte_op:      return Operators::gt_op;
                case Operators::lt_op:       return Operators::gte_op;
                case Operators::gte_op:      return Operators::lt_op;
                case Operators::gt_op:       return Operators::lte_op;
                default: break;
            }
            return Operators::unknown_op;
        }

        static constexpr KWList joinModifiers = {Keywords::outer_kw, Keywords::left_kw, Keywords::right_kw, Keywords::inner_kw};
    };




}
#endif /* Helpers_h */
