#include "litedb/lite_db_engine.hpp"

#include <memory>

using namespace litedb;

int main()
{
    auto engine = std::make_unique<LiteDbEngine>();

    engine->Execute("CREATE DATABASE testdb;");
    engine->Execute("USE testdb;");
    engine->Execute("CREATE TABLE users (id INT, age INT, name CHAR(100));");

    return 0;
}