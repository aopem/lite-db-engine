#ifndef __token_trie_HPP__
#define __token_trie_HPP__

#include "token.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace litedb::query
{
    class TokenTrie
    {
    public:
        TokenTrie();
        TokenTrie(const TokenTrie &) = delete;
        TokenTrie &operator=(const TokenTrie &) = delete;

        void Insert(std::string value);
        std::shared_ptr<Token> Search(std::string value);
        static std::shared_ptr<TokenTrie> GetInstance()
        {
            static std::shared_ptr<TokenTrie> instance = std::make_shared<TokenTrie>();
            return instance;
        }

    private:
        std::shared_ptr<Token> _root;
    };
};

#endif // __token_trie_HPP__