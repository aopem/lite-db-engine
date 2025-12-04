#include "litedb/lite_db_engine.hpp"
#include "query/lexer.hpp"
#include "query/parser.hpp"
#include "query/query_executor.hpp"

#include <iostream>

namespace litedb
{
    Result<void> LiteDbEngine::Execute(const std::string &sql)
    {
        // lex and parse
        Lexer lexer{sql};
        auto parse_result = _parser.Parse(lexer);
        if (!parse_result.success)
        {
            BOOST_LOG_TRIVIAL(error) << "Parse error: " << parse_result.error_message;
            return Result<void>::Error(parse_result.error_message);
        }

        // execute statement
        parse_result.value->Accept(_executor);
        return Result<void>::Ok();
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
            if (!std::getline(std::cin, user_sql_statement))
            {
                std::cout << "\nExiting..." << std::endl;
                break;
            }
            std::cout << std::endl;

            // execute (errors are handled internally)
            auto result = Execute(user_sql_statement);
            if (!result.success)
            {
                std::cout << "Error: " << result.error_message << std::endl;
            }
        }
    }
};