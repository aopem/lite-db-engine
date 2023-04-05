#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__

#include <string>
#include <unordered_map>

namespace litedb
{
    enum class symbol_e
    {
        IDENTIFIER,

        DATA_TYPE_INT,
        DATA_TYPE_FLOAT,
        DATA_TYPE_CHAR,

        LITERAL_INT,
        LITERAL_STRING,

        PUNCTUATOR_SEMICOLON,
        PUNCTUATOR_PLUS,
        PUNCTUATOR_MINUS,
        PUNCTUATOR_ASTERISK,
        PUNCTUATOR_EQUALS,
        PUNCTUATOR_LPAREN,
        PUNCTUATOR_RPAREN,
        PUNCTUATOR_COMMA,
        PUNCTUATOR_EOF,

        KEYWORD_CREATE_DATABASE,
        KEYWORD_DROP_DATABASE,
        KEYWORD_CREATE_TABLE,
        KEYWORD_DROP_TABLE,
        KEYWORD_SELECT,
        KEYWORD_FROM,
        KEYWORD_UPDATE,
        KEYWORD_SET,
        KEYWORD_DELETE,
        KEYWORD_USE,
        KEYWORD_SHOW,
        KEYWORD_SHOW_DATABASES,
        KEYWORD_NODE,

        KEYWORD_INVALID,

        INVALID
    };

    static std::unordered_map<symbol_e, std::string> symbol_e_map{
        { symbol_e::IDENTIFIER, std::string("IDENTIFIER") },

        { symbol_e::DATA_TYPE_INT, std::string("DATA_TYPE_INT") },
        { symbol_e::DATA_TYPE_FLOAT, std::string("DATA_TYPE_FLOAT") },
        { symbol_e::DATA_TYPE_CHAR, std::string("DATA_TYPE_CHAR") },

        { symbol_e::LITERAL_INT, std::string("LITERAL_INT") },
        { symbol_e::LITERAL_STRING, std::string("LITERAL_STRING") },

        { symbol_e::PUNCTUATOR_SEMICOLON, std::string("PUNCTUATOR_SEMICOLON") },
        { symbol_e::PUNCTUATOR_PLUS, std::string("PUNCTUATOR_PLUS") },
        { symbol_e::PUNCTUATOR_MINUS, std::string("PUNCTUATOR_MINUS") },
        { symbol_e::PUNCTUATOR_ASTERISK, std::string("PUNCTUATOR_ASTERISK") },
        { symbol_e::PUNCTUATOR_EQUALS, std::string("PUNCTUATOR_EQUALS") },
        { symbol_e::PUNCTUATOR_LPAREN, std::string("PUNCTUATOR_LPAREN") },
        { symbol_e::PUNCTUATOR_RPAREN, std::string("PUNCTUATOR_RPAREN") },
        { symbol_e::PUNCTUATOR_COMMA, std::string("PUNCTUATOR_COMMA") },
        { symbol_e::PUNCTUATOR_EOF, std::string("PUNCTUATOR_EOF") },

        { symbol_e::KEYWORD_CREATE_DATABASE, std::string("KEYWORD_CREATE_DATABASE") },
        { symbol_e::KEYWORD_DROP_DATABASE, std::string("KEYWORD_DROP_DATABASE") },
        { symbol_e::KEYWORD_CREATE_TABLE, std::string("KEYWORD_CREATE_TABLE") },
        { symbol_e::KEYWORD_DROP_TABLE, std::string("KEYWORD_DROP_TABLE") },
        { symbol_e::KEYWORD_SELECT, std::string("KEYWORD_SELECT") },
        { symbol_e::KEYWORD_FROM, std::string("KEYWORD_FROM") },
        { symbol_e::KEYWORD_UPDATE, std::string("KEYWORD_UPDATE") },
        { symbol_e::KEYWORD_SET, std::string("KEYWORD_SET") },
        { symbol_e::KEYWORD_DELETE, std::string("KEYWORD_DELETE") },
        { symbol_e::KEYWORD_USE, std::string("KEYWORD_USE") },
        { symbol_e::KEYWORD_SHOW, std::string("KEYWORD_SHOW") },
        { symbol_e::KEYWORD_SHOW_DATABASES, std::string("KEYWORD_SHOW_DATABASES") },
        { symbol_e::KEYWORD_NODE, std::string("KEYWORD_NODE") },

        { symbol_e::KEYWORD_INVALID, std::string("KEYWORD_INVALID") },

        { symbol_e::INVALID, std::string("INVALID") },
    };

    static std::unordered_map<std::string, symbol_e> symbol_map{
        { std::string(";"), symbol_e::PUNCTUATOR_SEMICOLON },
        { std::string("+"), symbol_e::PUNCTUATOR_PLUS },
        { std::string("-"), symbol_e::PUNCTUATOR_MINUS },
        { std::string("*"), symbol_e::PUNCTUATOR_ASTERISK },
        { std::string("="), symbol_e::PUNCTUATOR_EQUALS },
        { std::string("("), symbol_e::PUNCTUATOR_LPAREN },
        { std::string(")"), symbol_e::PUNCTUATOR_RPAREN },
        { std::string(","), symbol_e::PUNCTUATOR_COMMA },

        { std::string("CREATE DATABASE"), symbol_e::KEYWORD_CREATE_DATABASE },
        { std::string("DROP DATABASE"), symbol_e::KEYWORD_DROP_DATABASE },
        { std::string("CREATE TABLE"), symbol_e::KEYWORD_CREATE_TABLE },
        { std::string("DROP TABLE"), symbol_e::KEYWORD_DROP_TABLE },
        { std::string("SELECT"), symbol_e::KEYWORD_SELECT },
        { std::string("FROM"), symbol_e::KEYWORD_FROM },
        { std::string("UPDATE"), symbol_e::KEYWORD_UPDATE },
        { std::string("SET"), symbol_e::KEYWORD_SET },
        { std::string("DELETE"), symbol_e::KEYWORD_DELETE },
        { std::string("USE"), symbol_e::KEYWORD_USE },
        { std::string("SHOW DATABASES"), symbol_e::KEYWORD_SHOW_DATABASES },
    };
};

#endif // __SYMBOL_HPP__