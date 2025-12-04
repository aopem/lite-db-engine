#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include "symbol.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace litedb
{
    class Token
    {
    public:
        std::unordered_map<std::string, std::shared_ptr<Token>> children;

        Token(
            std::string value,
            symbol_e symbol_t);
        bool IsLiteral();
        bool IsDataType();
        bool IsPunctuator();
        bool IsKeyword();
        bool IsKeywordNode();
        bool IsInvalidKeyword();
        bool IsIdentifier();
        symbol_e GetType();
        std::string GetValue();
        void SetType(symbol_e symbol_t);

    protected:
        std::string _value;
        symbol_e _symbol_t;
    };
};

#endif // __TOKEN_HPP__