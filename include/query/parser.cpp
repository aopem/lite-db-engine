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
        _builder_factory->RegisterBuilder<UseNodeBuilder>(symbol_e::KEYWORD_USE);
        _builder_factory->RegisterBuilder<ShowDatabasesNodeBuilder>(symbol_e::KEYWORD_SHOW_DATABASES);
    }

    void Parser::ThrowParserError(std::string expected, std::shared_ptr<Token> actual)
    {
        auto error_msg = "Expected '" + expected + "', got symbol '" + actual->GetValue() + \
            "' of type '" + symbol_e_map[actual->GetType()] + "'";
        BOOST_LOG_TRIVIAL(error) << error_msg;
        throw std::exception();
    }

    bool Parser::Expect(symbol_e expected, std::shared_ptr<Token> actual, bool throw_errors)
    {
        if (actual->GetType() != expected)
        {
            if (throw_errors)
            {
                ThrowParserError(symbol_e_map[expected], actual);
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
            ThrowParserError(expected_elements, actual);
        }
    }

    data_type_t Parser::ParseDataType(Lexer& lexer, std::unique_ptr<CreateTableNodeBuilder>& builder)
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
                data_type.length = std::stoi(lexer.GetNextToken()->GetValue());
                lexer.GetNextToken();
                break;

            default:
                ThrowParserError("data type identifier", token);
                break;
        }

        return data_type;
    }

    std::shared_ptr<AstNode> Parser::Parse(Lexer& lexer)
    {
        auto token = lexer.GetNextToken();

        BOOST_LOG_TRIVIAL(info) << "Parsing token type: " << symbol_e_map[token->GetType()];
        auto builder = _builder_factory->CreateBuilder(token->GetType());
        switch (token->GetType())
        {
            case symbol_e::KEYWORD_CREATE_DATABASE:
                return ParseCreateDatabase(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_DROP_DATABASE:
                return ParseDropDatabase(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_CREATE_TABLE:
                return ParseCreateTable(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_SELECT:
                return ParseSelect(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_UPDATE:
                return ParseUpdate(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_DELETE:
                return ParseDelete(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_USE:
                return ParseUse(lexer, std::move(builder));
                break;

            case symbol_e::KEYWORD_SHOW_DATABASES:
                return ParseShowDatabases(lexer, std::move(builder));
                break;

            default:
                auto error_msg = "Unknown symbol '" + token->GetValue() + "' found";
                throw std::invalid_argument(error_msg);
                break;
        }
    }

    std::shared_ptr<AstNode> Parser::ParseCreateDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<CreateDatabaseNodeBuilder> builder(static_cast<CreateDatabaseNodeBuilder*>(builder_ptr.release()));

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseDropDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<DropDatabaseNodeBuilder> builder(static_cast<DropDatabaseNodeBuilder*>(builder_ptr.release()));

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseCreateTable(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<CreateTableNodeBuilder> builder(static_cast<CreateTableNodeBuilder*>(builder_ptr.release()));

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
                ThrowParserError("column data type", lexer.Peek());
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

    std::shared_ptr<AstNode> Parser::ParseSelect(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        // get proper builder
        std::unique_ptr<SelectNodeBuilder> builder(static_cast<SelectNodeBuilder*>(builder_ptr.release()));

        if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_ASTERISK)
        {
            // create select node with all columns selected
            auto column = lexer.GetNextToken()->GetValue();
            builder->AddColumn(column);
        }
        else
        {
            // add all column identifiers
            while (lexer.Peek()->IsIdentifier() ||
                lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
            {
                Expect(symbol_e::IDENTIFIER, lexer.Peek());
                auto column = lexer.GetNextToken()->GetValue();
                builder->AddColumn(column);

                // read comma token if present
                Expect({ symbol_e::PUNCTUATOR_COMMA, symbol_e::KEYWORD_FROM }, lexer.Peek());
                if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
                {
                    lexer.GetNextToken();
                }
            }
        }

        // get FROM token
        Expect(symbol_e::KEYWORD_FROM, lexer.Peek());
        lexer.GetNextToken();

        // then get table identifier
        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseUpdate(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        // get proper builder
        std::unique_ptr<UpdateNodeBuilder> builder(static_cast<UpdateNodeBuilder*>(builder_ptr.release()));

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

    std::shared_ptr<AstNode> Parser::ParseDelete(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<DeleteNodeBuilder> builder(static_cast<DeleteNodeBuilder*>(builder_ptr.release()));

        // get FROM token
        Expect(symbol_e::KEYWORD_FROM, lexer.Peek());
        lexer.GetNextToken();

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseUse(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<UseNodeBuilder> builder(static_cast<UseNodeBuilder*>(builder_ptr.release()));

        Expect(symbol_e::IDENTIFIER, lexer.Peek());
        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseShowDatabases(Lexer& lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<ShowDatabasesNodeBuilder> builder(static_cast<ShowDatabasesNodeBuilder*>(builder_ptr.release()));

        // ensure nothing after SHOW DATABASES command
        Expect({ symbol_e::PUNCTUATOR_SEMICOLON, symbol_e::PUNCTUATOR_EOF }, lexer.Peek());

        return builder->Build();
    }
};