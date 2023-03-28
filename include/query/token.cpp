#include "token.hpp"

namespace engine::query
{
    Token::Token(
        std::string value,
        symbol_e symbol_t
    ) : _value(value),
        _symbol_t(symbol_t) {}

    bool Token::IsLiteral()
    {
        return _symbol_t >= symbol_e::DATA_TYPE_INT &&
            _symbol_t <= symbol_e::DATA_TYPE_CHAR;
    }

    bool Token::IsDataType()
    {
        return _symbol_t >= symbol_e::LITERAL_INT &&
            _symbol_t <= symbol_e::LITERAL_STRING;
    }

    bool Token::IsPunctuator()
    {
        return _symbol_t >= symbol_e::PUNCTUATOR_SEMICOLON &&
            _symbol_t <= symbol_e::PUNCTUATOR_EQUALS;
    }

    bool Token::IsKeyword()
    {
        return _symbol_t >= symbol_e::KEYWORD_CREATE_DATABASE &&
            _symbol_t <= symbol_e::KEYWORD_NODE;
    }

    bool Token::IsInvalidKeyword()
    {
        return _symbol_t == symbol_e::KEYWORD_INVALID;
    }

    symbol_e Token::GetType()
    {
        return _symbol_t;
    }

    std::string Token::GetValue()
    {
        return _value;
    }

    void Token::SetType(symbol_e language_element_t)
    {
        _symbol_t = language_element_t;
    }
}