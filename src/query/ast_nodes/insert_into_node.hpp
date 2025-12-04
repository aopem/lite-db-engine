#ifndef __INSERT_INTO_NODE_HPP__
#define __INSERT_INTO_NODE_HPP__

#include "../ast_node_visitor.hpp"
#include "ast_node.hpp"
#include "../token.hpp"

#include <string>
#include <vector>
#include <memory>

namespace litedb
{
    class InsertIntoNode : public AstNode, public std::enable_shared_from_this<InsertIntoNode>
    {
    public:
        std::string table;
        std::vector<std::string> columns;
        std::vector<std::shared_ptr<Token>> values;

        void Accept(const std::shared_ptr<AstNodeVisitor> &visitor) override;
    };
};

#endif // __INSERT_INTO_NODE_HPP__
