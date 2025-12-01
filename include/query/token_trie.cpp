#include "token_trie.hpp"
#include "symbol.hpp"

#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>

namespace litedb
{
    TokenTrie::TokenTrie()
    {
        _root = std::make_shared<Token>("", symbol_e::KEYWORD_INVALID);

        // initialize tree with symbol_map by inserting all keywords
        for (auto& [symbol, _] : symbol_map)
        {
            Insert(symbol);
        }
    }

    void TokenTrie::Insert(std::string value)
    {
        BOOST_LOG_TRIVIAL(debug) << "Inserting value: " << value;

        // split value into separate keywords if > 1 word
        std::vector<std::string> keywords;
        boost::split(keywords, value, boost::is_any_of(" "));

        // add to tree
        auto curr_token = _root;
        std::string curr_word = "";
        for (auto& word : keywords)
        {
            auto type = symbol_e::KEYWORD_NODE;
            curr_word += word;
            if (symbol_map.count(curr_word) != 0)
            {
                type = symbol_map[curr_word];
            }

            // insert new token, then advance to new token
            BOOST_LOG_TRIVIAL(debug) << "Inserting: " << curr_word << ", Type: " << symbol_e_map[type];
            auto new_token = std::make_shared<Token>(curr_word, type);
            curr_token->children.insert({ word, new_token });
            curr_token = curr_token->children[word];

            // add a space to curr_word
            curr_word += " ";
        }
    }

    std::shared_ptr<Token> TokenTrie::Search(std::string value)
    {
        // split value into separate keywords if > 1
        std::vector<std::string> keywords;
        boost::split(keywords, value, boost::is_any_of(" "));

        auto curr_token = _root;
        for (auto& word : keywords)
        {
            // if keyword does not exist, return an invalid token
            // it is possible this token could be a string literal, etc.
            if (curr_token->children.count(word) == 0)
            {
                return std::make_shared<Token>(value, symbol_e::KEYWORD_INVALID);
            }

            curr_token = curr_token->children[word];
        }

        return curr_token;
    }
};