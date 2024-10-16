//
//  Database.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <fstream>
#include "storage/Storage.hpp"
#include "Table.hpp"
#include "Index.hpp"


namespace ECE141 {

    //db should be related to storage somehow...

    class Table;

    class Database : std::enable_shared_from_this<Database> {
    public:
        friend class SQLProcessor;
        Database();
        Database(const std::string& aName, AccessMode);

        virtual ~Database();
        StatusResult dump(std::ostream &anOutput); //debug...
        StatusResult drop();
        std::string getName() const { return name; }

        Table* getTable(const std::string& tableName);
        StatusResult createTable(Table&& table);
        StatusResult dropTable(std::string tableName);

        using TableCallback = std::function<StatusResult(std::string, Table&)>;
        StatusResult forEachTable(TableCallback aCallback);

        void setSharedPtr(std::shared_ptr<Database> aPtr);

        Storage storage; // ?
    protected:
        StatusResult updateIndex();
        StatusResult loadIndex();

        std::string name;
        std::map<std::string, Table> tables;
        StringIndex tableIndex;
        bool changed;  //might be helpful, or ignore if you prefer.

        std::weak_ptr<Database> thisPtr; // ???
    };

}
#endif /* Database_hpp */
