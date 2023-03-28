#include "lexer.hpp"

namespace engine::query
{
    Lexer::Lexer(
        std::string sql_statement
    ) : _sql_statement(sql_statement),
        _position(0),
        _symbols()
    {
        _token_trie = TokenTrie::GetInstance();

        // cleanup _sql_statement
        CleanStatement(_sql_statement);

        // split the _sql_statement by space delimiter
        // and save in _symbols vector
        boost::split(_symbols, _sql_statement, boost::is_any_of(" "));
    }

    std::shared_ptr<Token> Lexer::GetNextToken()
    {
        // create token from stored _symbols
        if (_position < _symbols.size())
        {
            auto token = _token_trie->Search(_symbols[_position]);
            if (token->IsInvalidKeyword())
            {
                // classify character string that is not a keyword
                ClassifyString(token);
            }
            else if (token->IsKeyword() && token->GetType() == symbol_e::KEYWORD_NODE)
            {
                ClassifyMultiwordKeyword(token);
            }

            // advance to next token
            _position++;

            BOOST_LOG_TRIVIAL(debug) << "Found token type: " << symbol_e_map[token->GetType()];
            return token;
        }
        else if (_position == _symbols.size())
        {
            return std::make_shared<Token>("", symbol_e::PUNCTUATOR_EOF);
        }
        else
        {
            return std::make_shared<Token>("", symbol_e::INVALID);
        }
    }

    std::shared_ptr<Token> Lexer::Peek()
    {
        BOOST_LOG_TRIVIAL(debug) << "Peeking next token...";

        // save position, but return next token
        auto position = _position;
        auto next_token = GetNextToken();
        _position = position;

        return next_token;
    }

    void Lexer::ClassifyString(std::shared_ptr<Token>& token)
    {
        auto value = token->GetValue();
        if (Utils::IsPositiveInt(value) || Utils::IsPositiveInt(value))
        {
            token->SetType(symbol_e::LITERAL_INT);
            return;
        }

        // convert to uppercase for switch statement
        boost::algorithm::to_upper(value);
        if (value == "INT")
        {
            token->SetType(symbol_e::DATA_TYPE_INT);
        }
        else if (value == "FLOAT")
        {
            token->SetType(symbol_e::DATA_TYPE_FLOAT);
        }
        else if (value == "CHAR")
        {
            token->SetType(symbol_e::DATA_TYPE_CHAR);
        }
        else
        {
            // if none of the above, classify as identifier
            token->SetType(symbol_e::IDENTIFIER);
        }
    }

    void Lexer::ClassifyMultiwordKeyword(std::shared_ptr<Token>& token)
    {
        // for processing muti-word keyword tokens
        while (_position < _symbols.size() && token->GetType() == symbol_e::KEYWORD_NODE)
        {
            // advance to next element
            _position++;

            // compound with current keyword
            auto keyword = token->GetValue() + " " + _symbols[_position];

            // search for and assign new compound keyword
            token = _token_trie->Search(keyword);
        }
    }

    void Lexer::CleanStatement(std::string& sql_statement)
    {
        // remove \t, \n, \r
        boost::regex r("[\r\n\t]");
        sql_statement = boost::regex_replace(sql_statement, r, " ");

        // insert spaces between important characters
        r = "([,()=+*;-])";
        sql_statement = boost::regex_replace(sql_statement, r, " $1 ");

        // remove extra spaces in between characters
        r = " +";
        sql_statement = boost::regex_replace(sql_statement, r, " ");

        // remove leading & trailing whitespace
        Utils::Trim(sql_statement);

        BOOST_LOG_TRIVIAL(info) << "Cleaned SQL statement: '" << sql_statement << "'";
    }
};