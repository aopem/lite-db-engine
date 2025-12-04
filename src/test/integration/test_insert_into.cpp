#include "litedb/lite_db_engine.hpp"

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    // setup database and table
    engine.Execute("CREATE DATABASE testdb;");
    engine.Execute("USE testdb;");
    engine.Execute("CREATE TABLE testtable (id INT, age INT, name CHAR(100));");

    // test INSERT INTO with column names
    engine.Execute("INSERT INTO testtable (id, age, name) VALUES (1, 25, 'ABC');");
    engine.Execute("INSERT INTO testtable (id, age, name) VALUES (2, 30, 'DEF');");

    // test INSERT INTO without column names
    engine.Execute("INSERT INTO testtable VALUES (3, 35, 'Bob');");

    // cleanup
    engine.Execute("DROP DATABASE testdb;");

    return 0;
}