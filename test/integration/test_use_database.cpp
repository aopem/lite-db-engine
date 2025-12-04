#include <litedb/lite_db_engine.hpp>

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    // setup database
    engine.Execute("CREATE DATABASE usedb;");

    // test USE to switch database
    engine.Execute("USE usedb;");

    // test creating table after USE (verifies context is set)
    engine.Execute("CREATE TABLE users (id INT, name CHAR(50));");

    // test insert after USE (verifies context persists)
    engine.Execute("INSERT INTO users VALUES (1, 'Alice');");

    // cleanup
    engine.Execute("DROP DATABASE usedb;");

    return 0;
}
