//
// Created by Daniel Ziper on 4/15/24.
//

#ifndef ECE141DB_TABULARVIEW_HPP
#define ECE141DB_TABULARVIEW_HPP

#include <iostream>
#include <functional>
#include "View.hpp"
#include "database/Table.hpp"
#include "misc/OrderedMap.hpp"

namespace ECE141 {

    //completely generic view, which you will subclass to show information
    class TabularView : public View {
    public:
        TabularView(Schema *aSchema, RowProvider* aProvider, size_t aPadding=2, bool printD=false)
                : theSchema(aSchema), theProvider(aProvider), padding(aPadding), printD(printD) {}
        TabularView(Table &aTable, size_t aPadding=2, bool printD=true)
            : theSchema(&aTable), theProvider(&aTable), padding(aPadding), printD(printD) {};

        void setLimit(size_t aLimit) {
            limit = aLimit;
        }

        bool show(std::ostream &aStream) override {
            computeColumnWidths();
            printDivider(aStream);
            printHeader(aStream);

            printDivider(aStream);

            size_t count = 0;
            theProvider->each([&](const Row& row) {
                printRow(aStream, row);
                if (printD) printDivider(aStream);
                count++;
                if (count == limit) {
                    return false;
                }
                return true;
            });

            if (!printD) printDivider(aStream);
            return true;
        }

        /* Reference
+----------------+--------+---------------+
| Type           | Id     | Extra         |
+----------------+--------+---------------+
| Meta           | 0      |               |
-----------------+--------+---------------+
| Schema         | 1      | Users         |
-----------------+--------+---------------+
| Data           | 1      | Users         |
-----------------+--------+---------------+
| Free           | 0      |               |
-----------------+--------+---------------+
| Data           | 3      | Users         |
-----------------+--------+---------------+
         */

    protected:
        bool computeColumnWidths() {
            for (auto item : theSchema->attributes) {
                columnWidths.push_back(item.first.size());
            }

            // Compute column widths
            theProvider->each([&](const Row& row){
                theSchema->attributes.orderedEach([&](size_t index, const std::string& key, const Attribute& attr) {
                    std::string valStr = Helpers::valueToString(row[index], attr.type);
                    columnWidths[index] =
                            std::max(valStr.size(), columnWidths[index]);
                    return true;
                });
                return true;
            });
            return true;
        }

        void printDivider(std::ostream &aStream) {
            theSchema->attributes.orderedEach([&](size_t index, const std::string& key, const Attribute& attr) {
                return true;
            });

            aStream << kCorner;
            for (auto width : columnWidths) {
                size_t rowWidth = 1 + width + padding;
                // space + width + pad + column
                aStream << std::string(rowWidth, kRow);
                aStream << std::string(1, kCorner);
            }
            aStream << "\n";
        }

        void printRow(std::ostream &aStream, const Row& aRow) {
            aStream << kCol;
            theSchema->attributes.orderedEach([&](size_t index, const std::string& key, const Attribute& attr) {
                aStream << kSpace;
                std::string valString = Helpers::valueToString(aRow[index], attr.type);
                aStream << valString;
                aStream << std::string(columnWidths[index] - valString.size() + padding, kSpace);
                aStream << kCol;
                return true;
            });
            aStream << "\n";
        }

        void printHeader(std::ostream &aStream) {
            aStream << kCol;
            theSchema->attributes.orderedEach([&](size_t index, const std::string& key, const Attribute& attr) {
                aStream << kSpace;
                aStream << key;
                aStream << std::string(columnWidths[index] - key.size() + padding, kSpace);
                aStream << kCol;
                return true;
            });
            aStream << "\n";
        }

        char kCorner = '+';
        char kRow = '-';
        char kCol = '|';
        char kSpace = ' ';

        Schema* theSchema;
        RowProvider* theProvider;
        size_t padding;
        size_t limit=0;
        bool printD;

//        OrderedMap<std::string, size_t> columnWidths;
        std::vector<size_t> columnWidths;
    };

    using ViewListener = std::function<void(View & aView)>;

}


#endif //ECE141DB_TABULARVIEW_HPP
