//
//  Database.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include "database/Database.hpp"
#include "misc/Config.hpp"

namespace ECE141 {

    Database::Database() {}
    Database::Database(const std::string &aName, AccessMode aMode)
            : storage(aName, aMode), name(aName), changed(true), tableIndex(aName) {
        std::string thePath = Config::getDBPath(name);
        //create for read/write
        if (std::holds_alternative<CreateFile>(aMode)) {
            INFO("Created a database: ", name)
            updateIndex(); // creates an empty index
        } else {
            INFO("Opened a database: ", name)
            loadIndex();
        }
    }

    StatusResult Database::updateIndex() {
        INFO("Start to update the database index")
        storage.writeStorable(tableIndex, 0, BlockType::metadata);
        INFO("Successfully updated the database index")
        return {Errors::noError};
    }

    StatusResult Database::loadIndex() {
        INFO("Start to load the database index")
        return storage.readStorable(tableIndex, 0);
    }

    Database::~Database() {
//        if (changed) {
//            //stuff to save?
//        }
        // TODO: storage.compact(); ?
        for (auto& tablePair : tables) {
            tablePair.second.close(this);
        }
        storage.close();
    }

    StatusResult Database::drop() {
        INFO("Dropping database")
        storage.close();
        std::filesystem::remove(Config::getDBPath(name));
        INFO("Successfully dropped database")
        return {Errors::noError};
    }

    // USE: Dump command for debug purposes...
    StatusResult Database::dump(std::ostream &anOutput) {
        return {Errors::notImplemented};
    }

    void Database::setSharedPtr(std::shared_ptr<Database> aPtr) {
        // TODO: This should really use shared_from_this() but couldn't get it to work...
        thisPtr = aPtr;
    }

    Table* Database::getTable(const std::string& tableName) {
        INFO("Start getting the table ", tableName)
        auto iter = tables.find(tableName);
        TRACE("Iterating through tables in the database")
        if (iter != tables.end()) {
            return &(iter->second);
        }
        if (auto blockNum = tableIndex.getBlock(tableName)) {
            INFO("Loading table ", tableName)
            Table t{tableName, thisPtr.lock()};
            storage.readStorable(t, *blockNum);
            // Note: table's index is not loaded, only the Schema.
            tables.insert({t.name, t});
            INFO("Successfully loaded the table ", tableName)
            return &tables[t.name];
        }
        return nullptr;
    }

    StatusResult Database::createTable(Table&& table) {
        INFO("Creating table ", table.name)
        if (tableIndex.contains(table.name)) {
            ERROR("tableExists when creating the table ", table.name)
            return {Errors::tableExists};
        }

        DEBUG("Writing the table to memory")
        uint32_t indexPos = storage.writeStorable(table.index);
        table.indexLocation = indexPos;
        uint32_t tablePos = storage.writeStorable(table);

        DEBUG("Add (", table.name, ", ", tablePos, ") to tableIndex ")
        tableIndex.add(table.name, tablePos);

        DEBUG("ADD (", table.name, ", theActualTable ", ") to tables")
        tables.insert({
            table.name,
            table
        });

        updateIndex();
        INFO("Successfully created table ", table.name)
        return {Errors::noError};
    }

}
