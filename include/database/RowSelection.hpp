//
// Created by Daniel Ziper on 5/7/24.
//

#ifndef ECE141DB_ROWSELECTION_HPP
#define ECE141DB_ROWSELECTION_HPP

#include <vector>
#include <queue>
#include "misc/BasicTypes.hpp"
#include "database/Schema.hpp"
#include "database/Table.hpp"
#include "database/RowProvider.hpp"

namespace ECE141 {

    class RowSelection : public RowProvider {
    public:
        RowSelection(const std::vector<TableField>& aQueryFields, const std::vector<Table*>& aTables) {
            tables = aTables;
            for (const auto& field : aQueryFields) {
                auto tableAndIdx = findTableForField(field);
                // Don't check for errors, we want to throw an exception if something is wrong here
                // This should be prevalidated by SelectStatement

                DataTypes type = (tableAndIdx.second->attributes.get(field.fieldName))->type;
                outputSchema.addAttribute({field.alias, type});
                // Add attributes in the same order that the getOutputRow will add them to the tableRow
            }
        };

        void add(Row* aRow) {
            selection.push_back(aRow);
            DEBUG("selection.push_back(aRow)")
        }

        StatusResult each(RowCallback aCallback) override {
            for (Row* row : selection) {
                if (!aCallback(*row)) return {Errors::noError};
            }
            return {Errors::noError};
        };

        size_t size() const override {
            return selection.size();
        }

        StatusResult limit(const std::vector<size_t>& limits) {
            if (limits.empty()) return {Errors::noError};
            else if (limits.size() == 1) {
                selection.erase(selection.begin() + limits[0], selection.end());
            } else {
                selection.erase(selection.begin() + limits[0] + limits[1], selection.end()); // Erase after end first
                selection.erase(selection.begin(), selection.begin() + limits[0]);
            }
            return {Errors::noError};
        };

        StatusResult orderBy(const std::vector<OrderByField> fields) {
            if (fields.empty()) return {Errors::noError};

            auto cmp = [&](OrderType left, OrderType right) {
                for (size_t idx = 0; idx < fields.size(); idx++) {
                    if (left.values[idx] == right.values[idx]) {
                        continue;
                    }
                    bool leftLess = left.values < right.values;
                    return (bool)(leftLess ^ fields[idx].ascending);
                }
                return true;
            };

            std::priority_queue<OrderType, std::vector<OrderType>, decltype(cmp)> sorter(cmp);

            for (auto row : selection) {
                OrderType item{row};
                for (const OrderByField& obf : fields) {
                    item.values.push_back(*row->getItem(obf.field));
                }
                sorter.push(item); // Auto sorted by PQueue
            }

            selection.clear();
            while (!sorter.empty()) {
                selection.push_back(sorter.top().row);
                sorter.pop();
            }
            return {Errors::noError};
        };

        Schema outputSchema;
    protected:
        struct OrderType {
            Row* row;
            std::vector<Value> values;
        };

        std::vector<Row*> selection;
        std::vector<Table*> tables;

        // Index, Table*
        std::pair<size_t, Table*> findTableForField(const TableField& field) {
            TRACE("findTableForField(", field.fieldName, ")")
            for (size_t idx = 0; idx < tables.size(); idx++) {
                if (field.table == tables[idx]->name) {
                    TRACE("Found, return pair (", idx, ", ", "tables[idx])")
                    return {idx, tables[idx]};
                }
            }
            TRACE("Not Found")
            return {0, nullptr};
        }

    };

}

#endif //ECE141DB_ROWSELECTION_HPP
