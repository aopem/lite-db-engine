#include "delete_node_builder.hpp"

namespace litedb
{
    DeleteNodeBuilder::DeleteNodeBuilder()
    {
        _node = std::make_shared<DeleteNode>();
    }

    DeleteNodeBuilder &DeleteNodeBuilder::SetTable(std::string &table)
    {
        _node->table = table;
        return *this;
    }

    std::shared_ptr<AstNode> DeleteNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> DeleteNodeBuilder::Clone()
    {
        return std::make_unique<DeleteNodeBuilder>();
    }
};