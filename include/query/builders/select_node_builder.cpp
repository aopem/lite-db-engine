#include "select_node_builder.hpp"

namespace engine::query
{
    SelectNodeBuilder::SelectNodeBuilder()
    {
        _node = std::make_unique<SelectNode>();
    }

    SelectNodeBuilder& SelectNodeBuilder::AddColumn(std::string& column)
    {
        _node->columns.push_back(column);
        return *this;
    }

    SelectNodeBuilder& SelectNodeBuilder::SetTable(std::string& table)
    {
        _node->table = table;
        return *this;
    }

    std::unique_ptr<AstNode> SelectNodeBuilder::Build()
    {
        return std::move(_node);
    }

    std::unique_ptr<NodeBuilder> SelectNodeBuilder::Clone()
    {
        return std::make_unique<SelectNodeBuilder>();
    }
};