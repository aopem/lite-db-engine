#ifndef __AST_NODE_HPP__
#define __AST_NODE_HPP__

#include "../ast_node_visitor.hpp"

#include <memory>

namespace litedb
{
    // forward declaration of visitor
    class AstNodeVisitor;

    class AstNode
    {
    public:
        AstNode() = default;
        virtual ~AstNode() = default;
        virtual void Accept(std::shared_ptr<AstNodeVisitor> visitor) = 0;
    };
};

#endif // __AST_NODE_HPP__