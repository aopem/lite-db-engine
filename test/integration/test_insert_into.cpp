#include <litedb/lite_db_engine.hpp>

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    // setup database and table
    engine.Execute("CREATE DATABASE testdb;");
    engine.Execute("USE testdb;");
    engine.Execute("CREATE TABLE testtable (id INT, age INT, name CHAR(100));");

    // test valid INSERT INTO with column names
    engine.Execute("INSERT INTO testtable (id, age, name) VALUES (1, 25, 'ABC');");
    engine.Execute("INSERT INTO testtable (id, age, name) VALUES (2, 30, 'XYZ');");

    // test valid INSERT INTO without column names
    engine.Execute("INSERT INTO testtable VALUES (3, 35, 'Charlie');");

    // test invalid INSERT without columns - type mismatch (string for INT column)
    engine.Execute("INSERT INTO testtable VALUES ('not_an_int', 25, 'Dave');");

    // test invalid INSERT - wrong type (string for INT column)
    engine.Execute("INSERT INTO testtable (id, age, name) VALUES ('not_an_int', 25, 'DEF');");

    // test invalid INSERT - unknown column
    engine.Execute("INSERT INTO testtable (id, unknown_col, name) VALUES (4, 25, 'KKKKKKKK');");

    // test invalid INSERT - column count mismatch
    engine.Execute("INSERT INTO testtable (id, age) VALUES (5, 25, 'AFOIHdfapFSipojfsafop');");

    // cleanup
    engine.Execute("DROP DATABASE testdb;");

    return 0;
}