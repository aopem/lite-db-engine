#include "query/lexer.hpp"
#include "query/token.hpp"
#include "query/parser.hpp"

#include <string>
#include <vector>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace engine::query;

void set_logging_filter()
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );
}

int main()
{
    set_logging_filter();

    std::vector<std::string> statements = {
        "CREATE DATABASE db;",
        "UPDATE mytable\nSET col = 120, abc='house';",
        "SELECT * FROM table",
        "CREATE   TABLE new_table ( \
            col1 int,               \
            test char(8),           \
            myFloat float,          \
            apples char(4)          \
        );",
        "DROP DATABASE drop_this",
        "DELETE FROM specialTable;"
    };

    Parser parser;
    for (auto statement : statements)
    {
        Lexer lexer(statement);
        auto ast_node = parser.Parse(lexer);
    }

    return 0;
}