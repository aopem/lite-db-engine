# lite-db-engine

A simple database engine written in C++.

## Features

### Relational Engine

Composed of:

-   Lexer
-   Parser
-   Query Executor

A SQL statement is passed to a lexer, which creates a series of tokens. The tokens are then moved to the parser, which generates Abstract Syntax Tree (AST) nodes. Then, the Query Executor uses these AST nodes to interact with the Storage Engine and actually execute the database commands.

### Storage Engine

** Currently in progress **

## Dependencies

-   CMake >= 3.25
-   Boost >= 1.81

## Usage

### MacOS (HomeBrew)

#### Build and Run Main Executable

```sh
brew install boost
cmake --preset macos-homebrew-intel && cmake --build build
./build/main
```

#### Build and Run Tests

To compile test executables, enable the `BUILD_TESTS` option:

```sh
cmake --preset macos-homebrew-intel -DBUILD_TESTS=ON && cmake --build build
```

Run all tests using CTest:

```sh
cd build && ctest
```

Or run individual test executables:

```sh
./build/test_create_table
```

### Terminal Output Example

```txt
[lite-db-engine] SQL Terminal
-----------------------------
>> CREATE DATABASE newdb
[2023-04-09 15:38:03.885123] [0x00007ff85fa1a340] [info]    Cleaned SQL statement: 'CREATE DATABASE newdb'
[2023-04-09 15:38:03.885165] [0x00007ff85fa1a340] [info]    Parsing token type: KEYWORD_CREATE_DATABASE
[2023-04-09 15:38:03.885184] [0x00007ff85fa1a340] [info]    Executing CREATE DATABASE
>>
```
