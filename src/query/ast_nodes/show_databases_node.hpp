#ifndef __SHOW_DATABASES_NODE_HPP__
#define __SHOW_DATABASES_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"

#include <memory>

namespace litedb
{
    class ShowDatabasesNode : public AstNode, public std::enable_shared_from_this<ShowDatabasesNode>
    {
    public:
        void Accept(std::shared_ptr<AstNodeVisitor> visitor) override;
    };
};

#endif // __SHOW_DATABASES_NODE_HPP__