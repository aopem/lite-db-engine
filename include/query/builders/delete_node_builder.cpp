#include "delete_node_builder.hpp"

namespace engine::query
{
    DeleteNodeBuilder::DeleteNodeBuilder()
    {
        _node = std::make_unique<DeleteNode>();
    }

    DeleteNodeBuilder& DeleteNodeBuilder::SetTable(std::string& table)
    {
        _node->table = table;
        return *this;
    }

    std::unique_ptr<AstNode> DeleteNodeBuilder::Build()
    {
        return std::move(_node);
    }
};