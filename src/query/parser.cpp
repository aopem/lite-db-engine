#include "parser.hpp"
#include <format>

namespace litedb
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
        _builder_factory->RegisterBuilder<InsertIntoNodeBuilder>(symbol_e::KEYWORD_INSERT_INTO);
        _builder_factory->RegisterBuilder<SelectNodeBuilder>(symbol_e::KEYWORD_SELECT);
        _builder_factory->RegisterBuilder<UpdateNodeBuilder>(symbol_e::KEYWORD_UPDATE);
        _builder_factory->RegisterBuilder<DeleteNodeBuilder>(symbol_e::KEYWORD_DELETE);
        _builder_factory->RegisterBuilder<UseNodeBuilder>(symbol_e::KEYWORD_USE);
        _builder_factory->RegisterBuilder<ShowDatabasesNodeBuilder>(symbol_e::KEYWORD_SHOW_DATABASES);
    }

    void Parser::SetError(std::string_view expected, std::shared_ptr<Token> actual)
    {
        _last_error = std::format("Expected '{}', got symbol '{}' of type '{}'",
                                  expected, actual->GetValue(), symbol_e_map[actual->GetType()]);
        BOOST_LOG_TRIVIAL(error) << _last_error;
    }

    bool Parser::Expect(symbol_e expected, std::shared_ptr<Token> actual)
    {
        if (actual->GetType() != expected)
        {
            SetError(symbol_e_map[expected], actual);
            return false;
        }
        return true;
    }

    bool Parser::Expect(std::unordered_set<symbol_e> expected, std::shared_ptr<Token> actual)
    {
        for (auto symbol : expected)
        {
            if (actual->GetType() == symbol)
            {
                return true;
            }
        }

        std::string expected_elements = "";
        for (auto symbol : expected)
        {
            expected_elements += symbol_e_map[symbol] + " ";
        }

        Utils::RightTrim(expected_elements);
        SetError(expected_elements, actual);
        return false;
    }

    Result<data_type_t> Parser::ParseDataType(Lexer &lexer)
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
            SetError("data type identifier", token);
            return Result<data_type_t>::Error(_last_error);
        }

        return Result<data_type_t>::Ok(data_type);
    }

    Result<std::shared_ptr<AstNode>> Parser::Parse(Lexer &lexer)
    {
        auto token = lexer.GetNextToken();

        BOOST_LOG_TRIVIAL(info) << "Parsing token type: " << symbol_e_map[token->GetType()];
        auto builder = _builder_factory->CreateBuilder(token->GetType());
        switch (token->GetType())
        {
        case symbol_e::KEYWORD_CREATE_DATABASE:
            return ParseCreateDatabase(lexer, std::move(builder));

        case symbol_e::KEYWORD_DROP_DATABASE:
            return ParseDropDatabase(lexer, std::move(builder));

        case symbol_e::KEYWORD_CREATE_TABLE:
            return ParseCreateTable(lexer, std::move(builder));

        case symbol_e::KEYWORD_INSERT_INTO:
            return ParseInsertInto(lexer, std::move(builder));

        case symbol_e::KEYWORD_SELECT:
            return ParseSelect(lexer, std::move(builder));

        case symbol_e::KEYWORD_UPDATE:
            return ParseUpdate(lexer, std::move(builder));

        case symbol_e::KEYWORD_DELETE:
            return ParseDelete(lexer, std::move(builder));

        case symbol_e::KEYWORD_USE:
            return ParseUse(lexer, std::move(builder));

        case symbol_e::KEYWORD_SHOW_DATABASES:
            return ParseShowDatabases(lexer, std::move(builder));

        default:
            auto error_msg = "Unknown symbol '" + token->GetValue() + "' found";
            BOOST_LOG_TRIVIAL(error) << error_msg;
            return Result<std::shared_ptr<AstNode>>::Error(error_msg);
        }
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseCreateDatabase(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<CreateDatabaseNodeBuilder> builder(static_cast<CreateDatabaseNodeBuilder *>(builder_ptr.release()));

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseDropDatabase(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<DropDatabaseNodeBuilder> builder(static_cast<DropDatabaseNodeBuilder *>(builder_ptr.release()));

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseCreateTable(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<CreateTableNodeBuilder> builder(static_cast<CreateTableNodeBuilder *>(builder_ptr.release()));

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        // get table name
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        if (!Expect(symbol_e::PUNCTUATOR_LPAREN, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        while (lexer.Peek()->IsIdentifier() || lexer.Peek()->IsDataType())
        {
            if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }
            auto column = lexer.GetNextToken()->GetValue();

            if (!lexer.Peek()->IsDataType())
            {
                SetError("column data type", lexer.Peek());
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }

            auto data_type_result = ParseDataType(lexer);
            if (!data_type_result.success)
            {
                return Result<std::shared_ptr<AstNode>>::Error(data_type_result.error_message);
            }

            builder->AddColumn(column, data_type_result.value);
            if (!Expect({symbol_e::PUNCTUATOR_COMMA, symbol_e::PUNCTUATOR_RPAREN}, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }

            lexer.GetNextToken();
        }

        if (!Expect(symbol_e::PUNCTUATOR_SEMICOLON, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        lexer.GetNextToken();

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseInsertInto(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<InsertIntoNodeBuilder> builder(static_cast<InsertIntoNodeBuilder *>(builder_ptr.release()));

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        // check for optional column list
        if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_LPAREN)
        {
            // consume left parenthesis
            lexer.GetNextToken();

            // parse column names
            while (lexer.Peek()->IsIdentifier() || lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
            {
                if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
                {
                    return Result<std::shared_ptr<AstNode>>::Error(_last_error);
                }
                auto column = lexer.GetNextToken()->GetValue();
                builder->AddColumn(column);

                if (!Expect({symbol_e::PUNCTUATOR_COMMA, symbol_e::PUNCTUATOR_RPAREN}, lexer.Peek()))
                {
                    return Result<std::shared_ptr<AstNode>>::Error(_last_error);
                }

                if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
                {
                    lexer.GetNextToken();
                }
            }

            if (!Expect(symbol_e::PUNCTUATOR_RPAREN, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }
            lexer.GetNextToken();
        }

        if (!Expect(symbol_e::KEYWORD_VALUES, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        if (!Expect(symbol_e::PUNCTUATOR_LPAREN, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        while (lexer.Peek()->IsLiteral() || lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
        {
            if (!Expect({symbol_e::LITERAL_INT, symbol_e::LITERAL_STRING}, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }
            auto value = lexer.GetNextToken();
            builder->AddValue(value);

            if (!Expect({symbol_e::PUNCTUATOR_COMMA, symbol_e::PUNCTUATOR_RPAREN}, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }

            if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
            {
                lexer.GetNextToken();
            }
        }

        if (!Expect(symbol_e::PUNCTUATOR_RPAREN, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        if (!Expect({symbol_e::PUNCTUATOR_SEMICOLON, symbol_e::PUNCTUATOR_EOF}, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseSelect(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<SelectNodeBuilder> builder(static_cast<SelectNodeBuilder *>(builder_ptr.release()));

        if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_ASTERISK)
        {
            auto column = lexer.GetNextToken()->GetValue();
            builder->AddColumn(column);
        }
        else
        {
            while (lexer.Peek()->IsIdentifier() || lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
            {
                if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
                {
                    return Result<std::shared_ptr<AstNode>>::Error(_last_error);
                }
                auto column = lexer.GetNextToken()->GetValue();
                builder->AddColumn(column);

                if (!Expect({symbol_e::PUNCTUATOR_COMMA, symbol_e::KEYWORD_FROM}, lexer.Peek()))
                {
                    return Result<std::shared_ptr<AstNode>>::Error(_last_error);
                }

                if (lexer.Peek()->GetType() == symbol_e::PUNCTUATOR_COMMA)
                {
                    lexer.GetNextToken();
                }
            }
        }

        if (!Expect(symbol_e::KEYWORD_FROM, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseUpdate(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<UpdateNodeBuilder> builder(static_cast<UpdateNodeBuilder *>(builder_ptr.release()));

        // get table name
        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        if (!Expect(symbol_e::KEYWORD_SET, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        while (lexer.Peek()->IsIdentifier() || lexer.Peek()->IsPunctuator() || lexer.Peek()->IsLiteral())
        {
            if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }
            auto column_name = lexer.GetNextToken()->GetValue();

            if (!Expect(symbol_e::PUNCTUATOR_EQUALS, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }
            lexer.GetNextToken();

            if (!Expect({symbol_e::LITERAL_INT, symbol_e::LITERAL_STRING}, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }
            auto value = lexer.GetNextToken();

            builder->AddUpdatedEntry({column_name, value});

            if (!Expect({symbol_e::PUNCTUATOR_COMMA, symbol_e::PUNCTUATOR_SEMICOLON, symbol_e::PUNCTUATOR_EOF}, lexer.Peek()))
            {
                return Result<std::shared_ptr<AstNode>>::Error(_last_error);
            }

            lexer.GetNextToken();
        }

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseDelete(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<DeleteNodeBuilder> builder(static_cast<DeleteNodeBuilder *>(builder_ptr.release()));

        if (!Expect(symbol_e::KEYWORD_FROM, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }
        lexer.GetNextToken();

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        auto table = lexer.GetNextToken()->GetValue();
        builder->SetTable(table);

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseUse(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<UseNodeBuilder> builder(static_cast<UseNodeBuilder *>(builder_ptr.release()));

        if (!Expect(symbol_e::IDENTIFIER, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        auto database = lexer.GetNextToken()->GetValue();
        builder->SetDatabase(database);

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }

    Result<std::shared_ptr<AstNode>> Parser::ParseShowDatabases(Lexer &lexer, std::unique_ptr<NodeBuilder> builder_ptr)
    {
        std::unique_ptr<ShowDatabasesNodeBuilder> builder(static_cast<ShowDatabasesNodeBuilder *>(builder_ptr.release()));

        if (!Expect({symbol_e::PUNCTUATOR_SEMICOLON, symbol_e::PUNCTUATOR_EOF}, lexer.Peek()))
        {
            return Result<std::shared_ptr<AstNode>>::Error(_last_error);
        }

        return Result<std::shared_ptr<AstNode>>::Ok(builder->Build());
    }
};