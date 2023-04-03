#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "ast_nodes/ast_nodes.hpp"
#include "builders/builders.hpp"
#include "lexer.hpp"
#include "symbol.hpp"
#include "data.hpp"

#include <memory>
#include <string>
#include <unordered_set>

#include <boost/log/trivial.hpp>

namespace engine::query
{
    class Parser
    {
        public:
            Parser();
            std::shared_ptr<AstNode> Parse(Lexer& lexer);

        private:
            std::unique_ptr<NodeBuilderFactory> _builder_factory;

            void RegisterBuilders();
            void ThrowParserError(std::string expected, std::string actual);
            bool Expect(symbol_e expected, std::shared_ptr<Token> actual, bool throw_errors = true);
            void Expect(std::unordered_set<symbol_e> expected, std::shared_ptr<Token> actual);
            data_type_t ParseDataType(Lexer& lexer, std::unique_ptr<CreateTableNodeBuilder>& builder);
            std::shared_ptr<AstNode> ParseCreateDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder> builder);
            std::shared_ptr<AstNode> ParseDropDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder> builder);
            std::shared_ptr<AstNode> ParseCreateTable(Lexer& lexer, std::unique_ptr<NodeBuilder> builder);
            std::shared_ptr<AstNode> ParseSelect(Lexer& lexer, std::unique_ptr<NodeBuilder> builder);
            std::shared_ptr<AstNode> ParseUpdate(Lexer& lexer, std::unique_ptr<NodeBuilder> builder);
            std::shared_ptr<AstNode> ParseDelete(Lexer& lexer, std::unique_ptr<NodeBuilder> builder);
    };
};

# endif // __PARSER_HPP__