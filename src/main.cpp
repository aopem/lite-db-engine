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
        "UPDATE mytable\nSET col = 120",
        "SELECT * FROM table",
        "CREATE   TABLE new_table ( \
            col1 int,               \
            test char(8),           \
            myFloat float,          \
            apples char(4)          \
        );",
        "DROP DATABASE drop_this"
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

    Lexer create_table_lexer(statements[3]);
    auto create_table_node = parser.Parse(create_table_lexer);
    for (auto& [col, data_type] : static_cast<CreateTableNode*>(create_table_node.get())->columns)
    {
        BOOST_LOG_TRIVIAL(info) << "Column: " << col << ", Data type: " << static_cast<int>(data_type.name) << ", Length: " << std::to_string(data_type.length);
    }

    Lexer drop_db_lexer(statements[4]);
    auto drop_db_node = parser.Parse(drop_db_lexer);
    BOOST_LOG_TRIVIAL(info) << "Database: " << static_cast<DropDatabaseNode*>(drop_db_node.get())->database;
    return 0;
}