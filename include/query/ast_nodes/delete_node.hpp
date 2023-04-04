#ifndef __DELETE_NODE_HPP__
#define __DELETE_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"

#include <string>
#include <memory>

namespace engine::query
{
    class DeleteNode : public AstNode, public std::enable_shared_from_this<DeleteNode>
    {
        public:
            std::string table;

            void Accept(std::shared_ptr<AstNodeVisitor> visitor) override;
    };
};

#endif // __DELETE_NODE_HPP__