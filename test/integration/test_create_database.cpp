#include <litedb/lite_db_engine.hpp>

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    // test creating a single database
    engine.Execute("CREATE DATABASE mydb;");

    // test creating multiple databases
    engine.Execute("CREATE DATABASE db1;");
    engine.Execute("CREATE DATABASE db2;");
    engine.Execute("CREATE DATABASE db3;");

    // test creating database that already exists (should skip)
    engine.Execute("CREATE DATABASE mydb;");

    // cleanup
    engine.Execute("DROP DATABASE mydb;");
    engine.Execute("DROP DATABASE db1;");
    engine.Execute("DROP DATABASE db2;");
    engine.Execute("DROP DATABASE db3;");

    return 0;
}
