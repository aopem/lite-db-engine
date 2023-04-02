#include "update_node_builder.hpp"

namespace engine::query
{
    UpdateNodeBuilder::UpdateNodeBuilder()
    {
        _node = std::make_unique<UpdateNode>();
    }

    UpdateNodeBuilder& UpdateNodeBuilder::SetTable(std::string& table)
    {
        _node->table = table;
        return *this;
    }

    UpdateNodeBuilder& UpdateNodeBuilder::AddColumn(std::pair<std::string, data_entry_t> column)
    {
        _node->columns.push_back(column);
        return *this;
    }

    std::unique_ptr<AstNode> UpdateNodeBuilder::Build()
    {
        return std::move(_node);
    }
};