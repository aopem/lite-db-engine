#include "litedb/lite_db_engine.hpp"
#include "query/lexer.hpp"
#include "query/parser.hpp"
#include "query/query_executor.hpp"

namespace litedb
{
    void LiteDbEngine::Run(std::istream &input_stream, std::ostream &output_stream)
    {
        std::string user_sql_statement = "";

        // read input
        output_stream << ">> ";
        std::getline(input_stream, user_sql_statement);
        output_stream << std::endl;

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
};