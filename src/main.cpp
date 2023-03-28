#include "query/lexer.hpp"
#include "query/token.hpp"
#include "query/parser.hpp"

#include <string>
#include <vector>

#include <boost/log/trivial.hpp>

using namespace engine::query;

int main()
{
    std::vector<std::string> statements = {
        "CREATE DATABASE db;",
        "UPDATE mytable\nSET col = 120",
        "SELECT * FROM table",
        "CREATE   TABLE new_table ( \
            col1 int,               \
            col2 char(8),           \
            col3 float,             \
            col4 char(4)            \
        );"
    };

    for (auto statement : statements)
    {
        Lexer lexer(statement);
        auto token = std::make_shared<Token>("", symbol_e::KEYWORD_INVALID);
        while (token->GetType() != symbol_e::INVALID &&
            token->GetType() != symbol_e::PUNCTUATOR_EOF)
        {
            token = lexer.GetNextToken();
        }
    }

    Parser parser;
    Lexer create_db_lexer(statements[0]);
    auto create_db_node = parser.Parse(create_db_lexer);
    BOOST_LOG_TRIVIAL(info) << "Database: " << static_cast<CreateDatabaseNode*>(create_db_node.get())->database;

    Lexer select_lexer(statements[2]);
    auto select_node = parser.Parse(select_lexer);
    BOOST_LOG_TRIVIAL(info) << "Table: " << static_cast<SelectNode*>(select_node.get())->table;
    return 0;
}