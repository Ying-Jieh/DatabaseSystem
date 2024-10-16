//
// Created by Daniel Ziper on 5/7/24.
//

#ifndef ECE141DB_ROWPROVIDER_HPP
#define ECE141DB_ROWPROVIDER_HPP

#include <functional>
#include "TableRow.hpp"

namespace ECE141 {
    using RowCallback = std::function<bool(const Row&)>;
    class RowProvider {
    public:
        virtual StatusResult each(RowCallback aCallback) = 0;
        virtual size_t size() const = 0;
    };
}

#endif //ECE141DB_ROWPROVIDER_HPP
