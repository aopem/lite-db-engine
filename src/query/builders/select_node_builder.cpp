#include "select_node_builder.hpp"

namespace litedb
{
    SelectNodeBuilder::SelectNodeBuilder()
    {
        _node = std::make_shared<SelectNode>();
    }

    SelectNodeBuilder &SelectNodeBuilder::AddColumn(const std::string &column)
    {
        _node->columns.push_back(column);
        return *this;
    }

    SelectNodeBuilder &SelectNodeBuilder::SetTable(const std::string &table)
    {
        _node->table = table;
        return *this;
    }

    std::shared_ptr<AstNode> SelectNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> SelectNodeBuilder::Clone()
    {
        return std::make_unique<SelectNodeBuilder>();
    }
};