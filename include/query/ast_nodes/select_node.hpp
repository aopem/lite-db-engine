#ifndef __SELECT_NODE_HPP__
#define __SELECT_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"

#include <vector>
#include <string>
#include <memory>

namespace litedb::query
{
    class SelectNode : public AstNode, public std::enable_shared_from_this<SelectNode>
    {
    public:
        std::string table;
        std::vector<std::string> columns;

        void Accept(std::shared_ptr<AstNodeVisitor> visitor) override;
    };
};

#endif // __SELECT_NODE_HPP__