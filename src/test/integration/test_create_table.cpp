#include "litedb/lite_db_engine.hpp"

using namespace litedb;

int main()
{
    auto engine = LiteDbEngine();

    engine.Execute("CREATE DATABASE testdb;");
    engine.Execute("USE testdb;");
    engine.Execute("CREATE TABLE testtable (id INT, age INT, name CHAR(100));");

    return 0;
}