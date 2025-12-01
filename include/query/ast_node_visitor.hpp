#ifndef __AST_NODE_VISITOR_HPP__
#define __AST_NODE_VISITOR_HPP__

#include "ast_nodes/ast_nodes.hpp"

#include <memory>

namespace litedb
{
    // node forward declarations
    class CreateDatabaseNode;
    class CreateTableNode;
    class DeleteNode;
    class DropDatabaseNode;
    class SelectNode;
    class UpdateNode;
    class UseNode;
    class ShowDatabasesNode;

    class AstNodeVisitor
    {
    public:
        AstNodeVisitor() = default;
        virtual ~AstNodeVisitor() = default;
        virtual void Visit(std::shared_ptr<CreateDatabaseNode> node) = 0;
        virtual void Visit(std::shared_ptr<CreateTableNode> node) = 0;
        virtual void Visit(std::shared_ptr<DeleteNode> node) = 0;
        virtual void Visit(std::shared_ptr<DropDatabaseNode> node) = 0;
        virtual void Visit(std::shared_ptr<SelectNode> node) = 0;
        virtual void Visit(std::shared_ptr<UpdateNode> node) = 0;
        virtual void Visit(std::shared_ptr<UseNode> node) = 0;
        virtual void Visit(std::shared_ptr<ShowDatabasesNode> node) = 0;
    };
};

#endif // __AST_NODE_VISITOR_HPP__