#ifndef __UPDATE_NODE_HPP__
#define __UPDATE_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"
#include "../token.hpp"

#include <string>
#include <vector>
#include <memory>

namespace litedb
{
    class UpdateNode : public AstNode, public std::enable_shared_from_this<UpdateNode>
    {
    public:
        std::string table;
        std::vector<std::pair<std::string, std::shared_ptr<Token>>> entries;

        void Accept(std::shared_ptr<AstNodeVisitor> visitor) override;
    };
};

#endif // __UPDATE_NODE_HPP__