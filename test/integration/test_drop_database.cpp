#include <litedb/lite_db_engine.hpp>

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    // setup databases
    engine.Execute("CREATE DATABASE dropdb1;");
    engine.Execute("CREATE DATABASE dropdb2;");

    // test dropping existing database
    engine.Execute("DROP DATABASE dropdb1;");

    // test dropping non-existent database (should warn)
    engine.Execute("DROP DATABASE nonexistent;");

    // cleanup
    engine.Execute("DROP DATABASE dropdb2;");

    return 0;
}
