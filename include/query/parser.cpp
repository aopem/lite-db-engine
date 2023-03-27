#include "parser.hpp"

namespace engine::query
{
    Parser::Parser()
    {
        _builder_factory = std::make_unique<NodeBuilderFactory>();
        RegisterBuilders();
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
            auto error_msg = "Expected database identifier, got '" + lexer.Peek()->GetValue() + "' instead";
            BOOST_LOG_TRIVIAL(error) << error_msg;
            throw std::invalid_argument(error_msg);
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
            auto error_msg = "Expected table identifier, got '" + lexer.Peek()->GetValue() + "' instead";
            BOOST_LOG_TRIVIAL(error) << error_msg;
            throw std::invalid_argument(error_msg);
        }

        // TODO: add logic for parsing columns of CREATE TABLE statement

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
            auto error_msg = "Expected SQL keyword 'FROM', got '" + lexer.Peek()->GetValue() + "' instead";
            BOOST_LOG_TRIVIAL(error) << error_msg;
            throw std::invalid_argument(error_msg);
        }

        // get FROM token, then get table identifier
        lexer.GetNextToken();
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return builder->Build();
    }

    void Parser::RegisterBuilders()
    {
        _builder_factory->RegisterBuilder<SelectNodeBuilder>(symbol_e::KEYWORD_SELECT);
        _builder_factory->RegisterBuilder<CreateDatabaseNodeBuilder>(symbol_e::KEYWORD_CREATE_DATABASE);
    }
};