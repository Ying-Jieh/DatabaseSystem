//
// Created by Daniel Ziper on 5/23/24.
//

#ifndef ECE141DB_JOIN_HPP
#define ECE141DB_JOIN_HPP

#include <memory>
#include "../include/database/RowProvider.hpp"
#include "JoinedRow.hpp"
#include "../Filters.hpp"
#include "../DBQuery.hpp"

namespace ECE141 {
    class Join : public RowProvider {
    public:
        size_t size() const override {
            return rows.size();
        }
        StatusResult each(RowCallback aCallback) override;
    protected:
        std::vector<JoinedRow> rows;
    };

    class NoJoin : public Join {
    public:
        NoJoin(const TableFieldList& fields, Table* table);
        ~NoJoin() {};
    };

    class RealJoin : public Join {
    public:
        RealJoin(const TableFieldList& fields, const std::vector<Table*>& tables, Filters& joinOn, bool right, bool outer=true);
        ~RealJoin() = default;
    protected:
        NullRow nullRows[Config::joinedRowSize];
        static std::vector<const TableRow*> createRowVec(bool left, const TableRow* r0, const TableRow* r1);
    };

    class LeftJoin : public RealJoin {
    public:
        LeftJoin(const TableFieldList& fields, const std::vector<Table*>& tables, Filters& joinOn, bool outer=true) :
                RealJoin(fields, tables, joinOn, false, outer) {};
    };
    class RightJoin : public RealJoin {
    public:
        RightJoin(const TableFieldList& fields, const std::vector<Table*>& tables, Filters& joinOn, bool outer=true) :
                RealJoin(fields, tables, joinOn, true, outer) {};
    };

    std::shared_ptr<Join> joinFactory(DBQuery& aQuery);
}

#endif //ECE141DB_JOIN_HPP
