#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include "token.hpp"
#include "token_trie.hpp"
#include "symbol.hpp"
#include "utils/utils.hpp"

#include <string>
#include <vector>
#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/log/trivial.hpp>

namespace engine::query
{
    class Lexer
    {
        public:
            Lexer(std::string sql_statement);
            std::shared_ptr<Token> GetNextToken();
            std::shared_ptr<Token> Peek();

        private:
            int _position;
            std::string _sql_statement;
            std::vector<std::string> _symbols;
            std::shared_ptr<TokenTrie> _token_trie;

            void ClassifyString(std::shared_ptr<Token>& token);
            void ClassifyMultiwordKeyword(std::shared_ptr<Token>& token);
            void CleanStatement(std::string& statement);
    };
};

#endif // __LEXER_HPP__