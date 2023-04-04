#include "query/lexer.hpp"
#include "query/token.hpp"
#include "query/parser.hpp"
#include "query/query_executor.hpp"

#include <string>
#include <vector>
#include <iostream>

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

    // create dependencies
    auto parser = std::make_unique<Parser>();
    auto executor = std::make_shared<QueryExecutor>();

    for (auto statement : statements)
    {
        Lexer lexer(statement);
        auto ast_node = parser->Parse(lexer);
    }

    std::cout << "[db-engine-lite] SQL Terminal" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    while (true)
    {
        std::string user_sql_statement = "";

        // read input
        std::cout << ">> ";
        std::getline(std::cin, user_sql_statement);

        // lex and parse
        Lexer lexer(user_sql_statement);
        auto node = parser->Parse(lexer);

        node->Accept(executor);
    }

    return 0;
}