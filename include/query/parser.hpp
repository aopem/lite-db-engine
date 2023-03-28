#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "ast_nodes/ast_nodes.hpp"
#include "builders/builders.hpp"
#include "lexer.hpp"
#include "symbol.hpp"
#include "data_type.hpp"

#include <memory>
#include <string>

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
            std::pair<std::string, data_type_t> ParseDataType(Lexer& lexer, CreateTableNodeBuilder* builder);
            std::shared_ptr<AstNode> ParseCreateDatabase(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder);
            std::shared_ptr<AstNode> ParseCreateTable(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder);
            std::shared_ptr<AstNode> ParseSelect(Lexer& lexer, std::unique_ptr<NodeBuilder>& builder);
    };
};

# endif // __PARSER_HPP__