#include "query/lexer.hpp"
#include "query/parser.hpp"
#include "query_executor.hpp"

#include <string>
#include <vector>
#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace litedb;

void set_logging_filter()
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::debug
    );
}

int main()
{
    set_logging_filter();

    // create dependencies
    auto parser = std::make_unique<Parser>();
    auto executor = std::make_shared<QueryExecutor>();

    std::cout << "[lite-db-engine] SQL Terminal" << std::endl;
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
        if (node == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "Unable to parse SQL statement, invalid syntax. Skipping execution.";
            continue;
        }

        // execute statement
        node->Accept(executor);
    }

    return 0;
}