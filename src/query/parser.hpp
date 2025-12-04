#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "ast_nodes/ast_nodes.hpp"
#include "builders/builders.hpp"
#include "lexer.hpp"
#include "symbol.hpp"
#include "data.hpp"
#include "utils/result.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>

#include <boost/log/trivial.hpp>

namespace litedb
{
    class Parser
    {
    public:
        Parser();
        Result<std::shared_ptr<AstNode>> Parse(Lexer &lexer);

    private:
        std::unique_ptr<NodeBuilderFactory> _builder_factory;
        std::string _last_error;

        void RegisterBuilders();
        void SetError(std::string_view expected, std::shared_ptr<Token> actual);
        bool Expect(symbol_e expected, std::shared_ptr<Token> actual);
        bool Expect(std::unordered_set<symbol_e> expected, std::shared_ptr<Token> actual);
        Result<data_type_t> ParseDataType(Lexer &lexer);
        Result<std::shared_ptr<AstNode>> ParseCreateDatabase(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseDropDatabase(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseCreateTable(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseInsertInto(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseSelect(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseUpdate(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseDelete(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseUse(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
        Result<std::shared_ptr<AstNode>> ParseShowDatabases(Lexer &lexer, std::unique_ptr<NodeBuilder> builder);
    };
};

#endif // __PARSER_HPP__