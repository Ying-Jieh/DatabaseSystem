- Create database "foo"
- Drop database "foo"
    - Even if its "open" in memory we can drop it
- Show databases
- Use database "foo"
    - Bring that database definition into memory (prepare for access)
    - Don't load the whole thing into memory!
    - Each database has its own file
- Dump database "foo"
    - Print out all of the tables and rows and indices (debugging tool)


```
create database foo
drop database foo
show database
use database foo
dump database foo
```

### DatabaseHandler
- Handle all PA2 functions
  - create
    - Create the empty db file
  - drop
    - Delete the db file
  - show
    - List all of the .db files in the db folder
  - use
    - Read db file into memory, parse index, etc
  - dump
    - Iterate through blocks of the database and print the header of each block?

### Database Object
- Name
- Index to Tables
  - Map from table name to block number of table definition


fooDatabase.db
barDatabase.db


### Table Object
- Name
- Table definition
  - Columns and types
- Row Index
  - Maps from key to block number of the row
- Key Constraints
  - Foreign Key, points to another table?


### Database File

Block Types
- Metadata
  - Lists tables and points to Schema definition blocks
  - Points to Indexes for each table
- Schema
  - Definition for one table
- Data
  - Row of data in some table
- Free
  - The user deleted the data that was at this block, so mark as free
- Index
  - points to Data rows for one table

Info in Block Header
- Block Type
- Identifier Hash (hash of table name, etc)
- nextBlock pointer


### Command Breakdown

`use database db;`
- Open database
- Read metadata (creates Database object in memory)

`select age, id, lastname from Users where name=Bob;`
- Lookup Schema block pointer for Users (from Metadata in Database obj)
- Lookup Index block pointer for Users (from metadata)
- Read Schema block, construct Table obj in memory
- Read Index block, construct Index obj in memory
- Apply query filter, collect result, 
- create Table object with results using Schema defined in query
- display table with TabularView


age int
