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
        _builder_factory->RegisterBuilder<CreateTableNodeBuilder>(symbol_e::KEYWORD_CREATE_TABLE);
        _builder_factory->RegisterBuilder<SelectNodeBuilder>(symbol_e::KEYWORD_SELECT);
    }

    void Parser::ThrowParserError(std::string expected, std::string actual)
    {
        auto error_msg = "Expected '" + expected + "', got '" + actual + "'";
        BOOST_LOG_TRIVIAL(error) << error_msg;
        throw std::exception();
    }

    std::pair<std::string, data_type_t> Parser::ParseDataType(Lexer& lexer, CreateTableNodeBuilder* builder)
    {
        auto token = lexer.GetNextToken();
        data_type_t data_type;
        switch (token->GetType())
        {
            case symbol_e::DATA_TYPE_INT:
                data_type.type = data_type_e::INT;
                data_type.length = sizeof(int);
                break;

            case symbol_e::DATA_TYPE_FLOAT:
                data_type.type = data_type_e::FLOAT;
                data_type.length = sizeof(float);
                break;

            case symbol_e::DATA_TYPE_CHAR:
                data_type.type = data_type_e::CHAR;
                data_type.length = token->GetValue().size();
                break;

            default:
                ThrowParserError("data type identifier", token->GetValue());
                break;
        }

        return { token->GetValue(), data_type };
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

            case symbol_e::KEYWORD_CREATE_TABLE:
                node = ParseCreateTable(lexer, builder);
                break;

            case symbol_e::KEYWORD_SELECT:
                node = ParseSelect(lexer, builder);
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

        if (lexer.Peek()->GetType() != symbol_e::IDENTIFIER)
        {
            ThrowParserError("database identifier", lexer.Peek()->GetValue());
        }

        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return builder->Build();
    }

    std::shared_ptr<AstNode> Parser::ParseCreateTable(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder_ptr)
    {
        auto builder = static_cast<CreateTableNodeBuilder*>(builder_ptr.get());

        if (lexer.Peek()->GetType() != symbol_e::IDENTIFIER)
        {
            ThrowParserError("table identifier", lexer.Peek()->GetValue());
        }

        // get table name
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        // parse table columns
        if (lexer.Peek()->GetType() != symbol_e::PUNCTUATOR_LPAREN)
        {
            ThrowParserError("(", lexer.Peek()->GetValue());
        }

        // consume left parentheses
        lexer.GetNextToken();

        // parse table columns
        while (lexer.Peek()->GetType() == symbol_e::IDENTIFIER ||
               lexer.Peek()->IsDataType())
        {
            if (lexer.Peek()->GetType() != symbol_e::IDENTIFIER)
            {
                ThrowParserError("column identifier", lexer.Peek()->GetValue());
            }
            auto column = lexer.GetNextToken()->GetValue();

            if (!lexer.Peek()->IsDataType())
            {
                ThrowParserError("column data type", lexer.Peek()->GetValue());
            }
            auto data_type = ParseDataType(lexer, builder);
        }

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
                auto column = lexer.GetNextToken()->GetValue();
                builder->AddColumn(column);
            }
        }

        // get table identifier
        if (lexer.Peek()->GetType() != symbol_e::KEYWORD_FROM)
        {
            ThrowParserError("FROM", lexer.Peek()->GetValue());
        }

        // get FROM token, then get table identifier
        lexer.GetNextToken();
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return builder->Build();
    }
};