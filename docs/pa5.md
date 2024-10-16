## PA 5

### DBQuery

- Table*
- Select Fields + Aliases?
- Limit Clause?
- Order Clause?
- Where Clause?
- Join Clause?
- Group By?

### SelectStatement

#### Parsing

- Produces DBQuery object
- Clauses can come in any order

#### Running

- RowSelection
  - Schema (defines output table)
  - SelectedPKeys vector<Key> (point to Keys in Table (cache))
  

- Get the table and schema definition from storage
- Load the Table's Index
- For each Row in Index
  - Load/Decode Row from file
  - Check Filter
    - Pass: Store in Table, store Key in SelectOutput 
    - Fail: If Cache not full, store in Cache (Table object)
- Group RowSelection
- Order RowSelection
- Limit RowSelection

#### Display Output

Use RowSelection schema to display in TabularView, given the proper selectedPKeys (already order)

### Storage
What actions do we need to handle?
1. Write Storable starting at block
2. Write Storable where free
3. Read Storable starting at block
4. Mark chain as free
5. Compact (future)

Primitives
1. Write Block
2. Read Block
3. For each (every block)
4. For each (list blocks)
5. Get next Free block
6. Move Block

### Reading/Writing Tables

1. `create table ...`
   1. Command handled by `Database`.
   2. Create Table object
   3. Store Table's Index
   4. Store Table (using indexLocation and schema definition)
2. `insert into`
    1. `getTable`
    2. Lazy load index
    3. `table.buildRow/table.addRow`
       1. Check constraints using index
3. `select from`
    1. Lazy load index
    2. For each row in index, check if in cache, then load into cache
        1. Mark required if passed filters
4. `update`
    1. Lazy load index
    2. For each row in index, check if in cache, then load into cache
        1. Mark required if passed filters
5. `delete`
    1. Lazy load index
    2. For each row in index, check if in cache, then load into cache
    3. If passes filters, mark block free, delete from cache, delete from index
6. `drop`
    1. For each block in index, mark chain as free
    2. Mark index chain as free
    3. Mark table definition as free
    4. Remove from database `tableIndex`
7. `getTable`
   1. Check if table already in `tables`
   2. If not, create the table object, and decode
   3. Don't load the index at this point?
