
### Parsing

```
ATTRIBUTE is...
  [name] [type] [optional args] 
  
CREATE TABLE is...
[create] [table] [(] 
  [attribute] [optional comma]
    ...more attributes separated by commas...
[)] [;]
```

Example
```sql
CREATE TABLE tasks (
  id INT AUTO_INCREMENT PRIMARY KEY,
  title VARCHAR(100) NOT NULL,
  price FLOAT DEFAULT 0.0,
  due_date TIMESTAMP, // 2020-04-15 10:11:12
  status BOOLEAN DEFAULT FALSE,
)
```

Idea: Use Functors that can be passed into `TokenParser`, these can maintain state and have more complicated parsing logic, and can even be nested!
- Functors are given the TokenParser to use
- After parsing, they will have some (optional) state, a `bool getState` and a `Errors error`
  - Parsing call from TokenParser can require getState or ignore it
  - Any `error` aborts the parsing and command handling process fully, and this error should be reported to the User.
- `TokenParser` checkpoints its own state before handing itself off to another `Parser`
  - If `Parser::getState = false` and getState is not required, restore checkpoint?
- Examples
  - `AttributeParser`: tries to parse and build an Attribute from a Create Table statement
  - `TypeParser`: tries to parse the next token into a Datatype?
  - `ValueParser`: given a `DataTypes` tries to parse the next token into a `Value`
    - Nested in `AttributeParser`, used to parse default values
  - `Statement` objects can be `Parsers` too??
