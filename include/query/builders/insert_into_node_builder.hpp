#ifndef __INSERT_INTO_NODE_BUILDER_HPP__
#define __INSERT_INTO_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../ast_nodes/insert_into_node.hpp"

namespace litedb
{
    class InsertIntoNodeBuilder : public NodeBuilder
    {
    public:
        InsertIntoNodeBuilder();
        InsertIntoNodeBuilder &SetTable(std::string &table);
        InsertIntoNodeBuilder &AddColumn(std::string &column);
        InsertIntoNodeBuilder &AddValue(std::shared_ptr<Token> value);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<InsertIntoNode> _node;
    };
};

#endif // __INSERT_INTO_NODE_BUILDER_HPP__
