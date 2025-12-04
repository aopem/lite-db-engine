#include <litedb/lite_db_engine.hpp>

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    // test SHOW DATABASES when empty
    engine.Execute("SHOW DATABASES;");

    // create some databases
    engine.Execute("CREATE DATABASE showdb1;");
    engine.Execute("CREATE DATABASE showdb2;");
    engine.Execute("CREATE DATABASE showdb3;");

    // test SHOW DATABASES with multiple databases
    engine.Execute("SHOW DATABASES;");

    // cleanup
    engine.Execute("DROP DATABASE showdb1;");
    engine.Execute("DROP DATABASE showdb2;");
    engine.Execute("DROP DATABASE showdb3;");

    // test SHOW DATABASES after cleanup
    engine.Execute("SHOW DATABASES;");

    return 0;
}
