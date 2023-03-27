#ifndef __LANGUAGE_ELEMENT_HPP__
#define __LANGUAGE_ELEMENT_HPP__

#include <string>
#include <unordered_map>

namespace engine::query
{
    enum class symbol_e
    {
        IDENTIFIER,

        LITERAL_INT,
        LITERAL_STRING,

        PUNCTUATOR_SEMICOLON,
        PUNCTUATOR_PLUS,
        PUNCTUATOR_MINUS,
        PUNCTUATOR_ASTERISK,
        PUNCTUATOR_EQUALS,
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
        KEYWORD_DROP,
        KEYWORD_NODE,

        KEYWORD_INVALID,

        INVALID
    };

    static std::unordered_map<symbol_e, std::string> symbol_e_map{
        { symbol_e::IDENTIFIER, std::string("IDENTIFIER") },

        { symbol_e::LITERAL_INT, std::string("LITERAL_INT") },
        { symbol_e::LITERAL_STRING, std::string("LITERAL_STRING") },

        { symbol_e::PUNCTUATOR_SEMICOLON, std::string("PUNCTUATOR_SEMICOLON") },
        { symbol_e::PUNCTUATOR_PLUS, std::string("PUNCTUATOR_PLUS") },
        { symbol_e::PUNCTUATOR_MINUS, std::string("PUNCTUATOR_MINUS") },
        { symbol_e::PUNCTUATOR_ASTERISK, std::string("PUNCTUATOR_ASTERISK") },
        { symbol_e::PUNCTUATOR_EQUALS, std::string("PUNCTUATOR_EQUALS") },
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
        { symbol_e::KEYWORD_DROP, std::string("KEYWORD_DROP") },
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

        { std::string("CREATE DATABASE"), symbol_e::KEYWORD_CREATE_DATABASE },
        { std::string("DROP DATABASE"), symbol_e::KEYWORD_DROP_DATABASE },
        { std::string("CREATE TABLE"), symbol_e::KEYWORD_CREATE_TABLE },
        { std::string("DROP TABLE"), symbol_e::KEYWORD_DROP_TABLE },
        { std::string("SELECT"), symbol_e::KEYWORD_SELECT },
        { std::string("FROM"), symbol_e::KEYWORD_FROM },
        { std::string("UPDATE"), symbol_e::KEYWORD_UPDATE },
        { std::string("SET"), symbol_e::KEYWORD_SET },
        { std::string("DELETE"), symbol_e::KEYWORD_DELETE },
        { std::string("DROP"), symbol_e::KEYWORD_DROP }
    };
};

#endif // __LANGUAGE_ELEMENT_HPP__