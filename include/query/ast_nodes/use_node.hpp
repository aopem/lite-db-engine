#ifndef __USE_NODE_HPP__
#define __USE_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"

#include <string>
#include <memory>

namespace litedb
{
    class UseNode : public AstNode, public std::enable_shared_from_this<UseNode>
    {
        public:
            std::string database;

            void Accept(std::shared_ptr<AstNodeVisitor> visitor) override;
    };
};

#endif // __USE_NODE_HPP__