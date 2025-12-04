#include "litedb/lite_db_engine.hpp"
#include "query/lexer.hpp"
#include "query/parser.hpp"
#include "query/query_executor.hpp"

#include <iostream>

namespace litedb
{
    void LiteDbEngine::Execute(const std::string &sql)
    {
        // lex and parse
        Lexer lexer(sql);
        auto node = _parser->Parse(lexer);
        if (node == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "Unable to parse SQL statement, invalid syntax. Skipping execution.";
            return;
        }

        // execute statement
        node->Accept(_executor);
    }

    void LiteDbEngine::RunInteractive()
    {
        std::cout << "[lite-db-engine] SQL Terminal" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        while (true)
        {
            std::string user_sql_statement = "";

            // read input
            std::cout << ">> ";
            std::getline(std::cin, user_sql_statement);
            std::cout << std::endl;

            // lex and parse
            Lexer lexer{user_sql_statement};
            auto node = _parser->Parse(lexer);
            if (node == nullptr)
            {
                BOOST_LOG_TRIVIAL(error) << "Unable to parse SQL statement, invalid syntax. Skipping execution.";
                return;
            }

            // execute statement
            node->Accept(_executor);
        }
    }
};