//
// Created by Daniel Ziper on 5/25/24.
//

#ifndef ECE141DB_EXECUTIONCONTEXT_HPP
#define ECE141DB_EXECUTIONCONTEXT_HPP

#include <utility>

#include "database/Database.hpp"
#include "view/View.hpp"

namespace ECE141 {
    struct ExecutionContext {
        ExecutionContext(std::shared_ptr<Database> db, ViewListener& aListener)
                : database(std::move(db)), viewListener(aListener) {}

        ExecutionContext(const ExecutionContext& aCopy) = default;
        ExecutionContext& operator=(const ExecutionContext& aContext) = delete;

        std::shared_ptr<Database> database;
        ViewListener& viewListener;
    };
}

#endif //ECE141DB_EXECUTIONCONTEXT_HPP
