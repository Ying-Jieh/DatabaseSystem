//
// Created by Daniel Ziper on 5/24/24.
//

#include "../include/misc/joins/Join.hpp"

using namespace ECE141;

std::shared_ptr<Join> ECE141::joinFactory(DBQuery& aQuery) {
    if (aQuery.joinKeywords.empty())
        return std::make_shared<NoJoin>(aQuery.selectedFields, aQuery.table[0]);
    if (aQuery.joinKeywords.size() > 2) return nullptr;
    bool outer = true;
    if (Helpers::in_array(aQuery.joinKeywords, Keywords::inner_kw)) outer = false;
    bool left = Helpers::in_array(aQuery.joinKeywords, Keywords::left_kw);
    bool right = Helpers::in_array(aQuery.joinKeywords, Keywords::right_kw);
    if (left)
        return std::make_shared<LeftJoin>(aQuery.selectedFields, aQuery.table, aQuery.joinOn, outer);
    if (right)
        return std::make_shared<RightJoin>(aQuery.selectedFields, aQuery.table, aQuery.joinOn, outer);
    return nullptr;
}


NoJoin::NoJoin(const TableFieldList &fields, Table *table) {
    table->cache.eachRequired([&](const IdentifierHash &, const TableRow &row, const CacheStatus &) {
        rows.push_back({
                               {const_cast<TableRow *>(&row)},
                               fields
                       });
        return true;
    });
};

StatusResult Join::each(RowCallback aCallback) {
    for (JoinedRow &row: rows) {
        if (!aCallback(row)) return {Errors::noError};
    }
    return {Errors::noError};
}


RealJoin::RealJoin(const ECE141::TableFieldList &fields, const std::vector<Table *> &tables, Filters& joinOn, bool right, bool outer) {
    if (tables.size() != Config::joinedRowSize) throw std::length_error("Invalid number of tables in join!");
    nullRows[0] = NullRow(tables[0]);
    nullRows[1] = NullRow(tables[1]);

    tables[right]->cache.eachRequired([&](const IdentifierHash &, const TableRow &row0, const CacheStatus &) {
        bool matched = false;
        tables[!right]->cache.eachRequired([&](const IdentifierHash &, const TableRow &row1, const CacheStatus &) {
            JoinedRow jr {
                createRowVec(right, &row0, &row1),
                fields
            };
            if (joinOn.matches(jr)) {
                rows.push_back(jr);
                matched = true;
            }
            return true;
        });
        if (!matched && outer) {
            JoinedRow jr {createRowVec(right, &row0, &nullRows[!right]), fields};
            rows.push_back(jr);
        }
        return true;
    });
}

inline std::vector<const TableRow*> RealJoin::createRowVec(bool left, const TableRow* row0, const TableRow* row1) {
    if (left) return {row0, row1};
    else return {row1, row0};
}