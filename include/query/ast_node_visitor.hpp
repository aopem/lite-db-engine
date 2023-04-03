#ifndef __AST_NODE_VISITOR_HPP__
#define __AST_NODE_VISITOR_HPP__

#include "ast_nodes/ast_nodes.hpp"

#include <memory>

namespace engine::query
{
    class AstNodeVisitor
    {
        public:
            virtual ~AstNodeVisitor() = default;
            virtual void visit(std::shared_ptr<CreateDatabaseNode> node) = 0;
            virtual void visit(std::shared_ptr<CreateTableNode> node) = 0;
            virtual void visit(std::shared_ptr<DeleteNode> node) = 0;
            virtual void visit(std::shared_ptr<DropDatabaseNode> node) = 0;
            virtual void visit(std::shared_ptr<SelectNode> node) = 0;
            virtual void visit(std::shared_ptr<UpdateNode> node) = 0;
    };
};

#endif // __AST_NODE_VISITOR_HPP__