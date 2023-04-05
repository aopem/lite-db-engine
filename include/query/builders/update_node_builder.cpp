#include "update_node_builder.hpp"

namespace litedb
{
    UpdateNodeBuilder::UpdateNodeBuilder()
    {
        _node = std::make_shared<UpdateNode>();
    }

    UpdateNodeBuilder& UpdateNodeBuilder::SetTable(std::string& table)
    {
        _node->table = table;
        return *this;
    }

    UpdateNodeBuilder& UpdateNodeBuilder::AddUpdatedEntry(std::pair<std::string, std::shared_ptr<Token>> entry)
    {
        _node->entries.push_back(entry);
        return *this;
    }

    std::shared_ptr<AstNode> UpdateNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> UpdateNodeBuilder::Clone()
    {
        return std::make_unique<UpdateNodeBuilder>();
    }
};