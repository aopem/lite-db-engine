#include "parser.hpp"

namespace engine::query
{
    Parser::Parser()
    {
        _builder_factory = std::make_unique<NodeBuilderFactory>();
        RegisterBuilders();
    }

    void Parser::RegisterBuilders()
    {
        _builder_factory->RegisterBuilder<CreateDatabaseNodeBuilder>(symbol_e::KEYWORD_CREATE_DATABASE);
        _builder_factory->RegisterBuilder<DropDatabaseNodeBuilder>(symbol_e::KEYWORD_DROP_DATABASE);
        _builder_factory->RegisterBuilder<CreateTableNodeBuilder>(symbol_e::KEYWORD_CREATE_TABLE);
        _builder_factory->RegisterBuilder<SelectNodeBuilder>(symbol_e::KEYWORD_SELECT);
        _builder_factory->RegisterBuilder<UpdateNodeBuilder>(symbol_e::KEYWORD_UPDATE);
        _builder_factory->RegisterBuilder<DeleteNodeBuilder>(symbol_e::KEYWORD_DELETE);
    }

    void Parser::ThrowParserError(std::string expected, std::string actual)
    {
        auto error_msg = "Expected '" + expected + "', got '" + actual + "'";
        BOOST_LOG_TRIVIAL(error) << error_msg;
        throw std::exception();
    }

    bool Parser::Expect(symbol_e expected, std::shared_ptr<Token> actual, bool throw_errors)
    {
        if (actual->GetType() != expected)
        {
            if (throw_errors)
            {
                ThrowParserError(symbol_e_map[expected], actual->GetValue());
            }

            return false;
        }

        return true;
    }

    void Parser::Expect(std::unordered_set<symbol_e> expected, std::shared_ptr<Token> actual)
    {
        bool symbol_matched = false;
        for (auto symbol : expected)
        {
            symbol_matched = symbol_matched || Expect(symbol, actual, false);
        }

        if (!symbol_matched)
        {
            std::string expected_elements = "";
            for (auto symbol : expected)
            {
                expected_elements += symbol_e_map[symbol] + " ";
            }

            // trim for output, then throw error
            Utils::RightTrim(expected_elements);
            ThrowParserError(expected_elements, actual->GetValue());
        }
    }

    data_type_t Parser::ParseDataType(Lexer& lexer, CreateTableNodeBuilder* builder)
    {
        auto token = lexer.GetNextToken();
        data_type_t data_type;
        switch (token->GetType())
        {
            case symbol_e::DATA_TYPE_INT:
                data_type.name = data_type_e::INT;
                data_type.length = sizeof(int);
                break;

            case symbol_e::DATA_TYPE_FLOAT:
                data_type.name = data_type_e::FLOAT;
                data_type.length = sizeof(float);
                break;

            case symbol_e::DATA_TYPE_CHAR:
                data_type.name = data_type_e::CHAR;

                // get left paren, read length, then read right paren
                lexer.GetNextToken();
                data_type.length = stoi(lexer.GetNextToken()->GetValue());
                lexer.GetNextToken();
                break;

            default:
                ThrowParserError("data type identifier", token->GetValue());
                break;
        }

        return data_type;
    }

    std::shared_ptr<AstNode> Parser::Parse(Lexer& lexer)
    {
        auto token = lexer.GetNextToken();
        auto node = std::make_shared<AstNode>();

        BOOST_LOG_TRIVIAL(info) << "Parsing token type: " << symbol_e_map[token->GetType()];
        auto builder = _builder_factory->CreateBuilder(token->GetType());
        switch (token->GetType())
        {
            case symbol_e::KEYWORD_CREATE_DATABASE:
                node = ParseCreateDatabase(lexer, builder);
                break;

            case symbol_e::KEYWORD_DROP_DATABASE:
                node = ParseDropDatabase(lexer, builder);
                break;

            case symbol_e::KEYWORD_CREATE_TABLE:
                node = ParseCreateTable(lexer, builder);
                break;

            case symbol_e::KEYWORD_SELECT:
                node = ParseSelect(lexer, builder);
                break;

            case symbol_e::KEYWORD_UPDATE:
                node = ParseUpdate(lexer, builder);
                break;

            case symbol_e::KEYWORD_DELETE:
                node = ParseDelete(lexer, builder);
                break;

            default:
                auto error_msg = "Unknown symbol '" + token->GetValue() + "' found";
                throw std::invalid_argument(error_msg);
                break;
        }

        return node;
    }

    std::shared_ptr<AstNode> Parser::ParseCreateDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        auto builder = static_cast<CreateDatabaseNodeBuilder*>(builder_ptr.get());

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseDropDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        auto builder = static_cast<DropDatabaseNodeBuilder*>(builder_ptr.get());

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseCreateTable(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        auto builder = static_cast<CreateTableNodeBuilder*>(builder_ptr.get());

        Expect(symbol_e::IDENTIFIER, lexer.Peek());

        // get table name
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        // consume left parentheses
        Expect(symbol_e::PUNCTUATOR_LPAREN, lexer.Peek());
        lexer.GetNextToken();

        // parse table columns
        while (lexer.Peek()->IsIdentifier() ||
            lexer.Peek()->IsDataType())
        {
            // read identifier
            Expect(symbol_e::IDENTIFIER, lexer.Peek());
            auto column = lexer.GetNextToken()->GetValue();

            // read data type
            if (!lexer.Peek()->IsDataType())
            {
                ThrowParserError("column data type", lexer.Peek()->GetValue());
            }
            auto data_type = ParseDataType(lexer, builder);

            // add column to create table node
            builder->AddColumn({ column, data_type });

            // read comma before next column type/closing right paren
            Expect({ symbol_e::PUNCTUATOR_COMMA, symbol_e::PUNCTUATOR_RPAREN }, lexer.Peek());
            lexer.GetNextToken();
        }

        // read final semicolon
        Expect(symbol_e::PUNCTUATOR_SEMICOLON, lexer.Peek());
        lexer.GetNextToken();

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseSelect(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        // get proper builder
        auto builder = static_cast<SelectNodeBuilder*>(builder_ptr.get());

        if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_ASTERISK)
        {
            // create select node with all columns selected
            auto column = lexer.GetNextToken()->GetValue();
            builder->AddColumn(column);
        }
        else
        {
            // add all column identifiers
            while (lexer.Peek()->GetType() == symbol_e::IDENTIFIER)
            {
                Expect(symbol_e::IDENTIFIER, lexer.Peek());
                auto column = lexer.GetNextToken()->GetValue();
                builder->AddColumn(column);
            }
        }

        // get FROM token
        Expect(symbol_e::KEYWORD_FROM, lexer.Peek());
        lexer.GetNextToken();

        // then get table identifier
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseUpdate(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        // get proper builder
        auto builder = static_cast<UpdateNodeBuilder*>(builder_ptr.get());

        // get table name
        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        // read SET token
        Expect(symbol_e::KEYWORD_SET, lexer.Peek());
        lexer.GetNextToken();

        while (lexer.Peek()->IsIdentifier() ||
            lexer.Peek()->IsPunctuator() ||
            lexer.Peek()->IsLiteral())
        {
            // read column name
            Expect(symbol_e::IDENTIFIER, lexer.Peek());
            auto column_name = lexer.GetNextToken()->GetValue();

            // read '=' sign
            Expect(symbol_e::PUNCTUATOR_EQUALS, lexer.Peek());
            lexer.GetNextToken();

            // read column new value
            Expect({ symbol_e::LITERAL_INT, symbol_e::LITERAL_STRING }, lexer.Peek());
            auto value = lexer.GetNextToken();

            builder->AddUpdatedEntry({ column_name, value });

            // read comma before next value, or ending semicolon/EOF
            Expect({ symbol_e::PUNCTUATOR_COMMA, symbol_e::PUNCTUATOR_SEMICOLON, symbol_e::PUNCTUATOR_EOF },
                lexer.Peek());
            lexer.GetNextToken();
        }

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseDelete(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        auto builder = static_cast<DeleteNodeBuilder*>(builder_ptr.get());

        // get FROM token
        Expect(symbol_e::KEYWORD_FROM, lexer.Peek());
        lexer.GetNextToken();

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return builder->Build();
    }
};