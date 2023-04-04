#ifndef __SHOW_DATABASES_NODE_HPP__
#define __SHOW_DATABASES_NODE_HPP__

#include "ast_node.hpp"
#include "../ast_node_visitor.hpp"

#include <memory>

namespace engine::query
{
    class ShowDatabasesNode : public AstNode, public std::enable_shared_from_this<ShowDatabasesNode>
    {
        public:
            void Accept(std::shared_ptr<AstNodeVisitor> visitor) override
            {
                visitor->Visit(shared_from_this());
            }
    };
};

#endif // __SHOW_DATABASES_NODE_HPP__