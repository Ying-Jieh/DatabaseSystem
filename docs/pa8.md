## PA 8

### Joins

- `RowSelection`/`TabularView` already supports combining multiple rows to display the output based on Schema
  - `RowSelection` uses the Table's cache to actually get row values - some extra work needed for creating NULL rows (currently not supported by cache?)
  - Maybe just add a special NULL row object in the cache designated by a specific ID?
- A new `Filters::matches(vector<TableRow>)` should be used to Join rows.
- `JoinClauseParser` needs to be implemented
- Add tables from all join clauses to DBQuery vector<Table*>
- Implement `Join.each` by calling a `Filter` as a requirement, and adding to RowSelection as callback
- `LeftJoin`/`RightJoin` basically have different `each` logic?
- `NoJoin` which just iterates the table?

### Cache

- Create `LRUCache<KeyT, ValueT>`, use `std::list<std::pair<KeyT, ValueT> + std::unordered_map<Key, list_iteraor>`?
  - get fast lookup and easy sorting.
    - Use the unordered_map to look up the value in the list, and use the list to keep the order of the elements
    - For `Block`, map<Key, Value> = map<blockNum, theActualBlock> 
    - For `Row`, map<Key, value> = map<std::pair<TableName (string), IdentifierHash>, Row or Row*>?
- Integrate BlockCache into Storage
  - Override `Storage::readBlock` to first check the cache, and if its not there, call `BlockIO::readBlock` and add it to the cache
  - For `writeBlock`, if in the cache, need to update the Value
- Later for RowCache extra credit?
  - Refactor current `RowCache` -> `RowCollection`?
  - Instead of using old `RowCache` in `forEachRow`, check if `cache.contains` first
