## Logger

1. ERROR: when StatusResult.error != Errors.error
2. WARN: potential issue that might lead to problem
3. INFO: when the process start and end, or something important you wanna mention
4. DEBUG: when the member variable is changed/declared, log the variable, or when meet if statement
5. TRACE: whatever detail you want

- Basically logging `ERROR` when theResult.error != noError
- `AppController.cpp`
    - `INFO` when starting handle input

- `BasicHandler.hpp`
    - `INFO` when start and successfully handling command
    - `TRACE` log the start and the end of handle()

- `DatabaseHandler.hpp`
    - `INFO` when start and successfully handling command
    - `TRACE` the start and the end of handle()

- `SQLProcessor.hpp`
    - `INFO` when start and successfully handling command
    - `TRACE` the start and the end of handle()

- `Database.cpp`
    - `INFO` whenever the function starts and ends
    - `DEBUG` when add something to the member variable, like tables and tableIndex

- `Schema.cpp`
    - `INFO` whenever the function starts and ends
    - `DEBUG` when add something to the member variable, like attributes
    - `TRACE` log what attribute name is encoded and decoded

- `Table.cpp`
    - `DEBUG` when we add `(id, startBlock)` to the `index`, when the `cache` is cleared.

- `Index.hpp`
    - `DEBUG` when add `(id, blockNum)` to the `items`, when decode and encode the block

- `TableRow.cpp`
    - `DEBUG` when there is if-statement, log which condition is going on

- `Storage.cpp`
    - `DEBUG` see if the `blockCache` contains the block

- `Parsers`
    - `INFO` when start and finish parsing
    - `DEBUG` log what is parsed

- `Statements`
    - `INFO` when start and finish running


## Refactor

We were passing the `AppController` to every `parser` to get the access to the `database` and the `viewListener`, now we create a class called `ExecutionContext` which wrap up the `database`
and the `viewListener`, so that we can only pass this down to the process instead of the whole controller to prevent any misuse of the members in the controller.