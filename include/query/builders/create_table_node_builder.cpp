#include "create_table_node_builder.hpp"

namespace engine::query
{
    CreateTableNodeBuilder::CreateTableNodeBuilder()
    {
        _node = std::make_unique<CreateTableNode>();
    }

    CreateTableNodeBuilder& CreateTableNodeBuilder::SetDatabase(std::string& database)
    {
        _node->database = database;
        return *this;
    }

    CreateTableNodeBuilder& CreateTableNodeBuilder::SetTable(std::string& table)
    {
        _node->table = table;
        return *this;
    }

    CreateTableNodeBuilder& CreateTableNodeBuilder::AddColumn(std::pair<std::string, data_type_t> column)
    {
        _node->columns.push_back(column);
        return *this;
    }

    std::unique_ptr<AstNode> CreateTableNodeBuilder::Build()
    {
        return std::move(_node);
    }

    std::unique_ptr<NodeBuilder> CreateTableNodeBuilder::Clone()
    {
        return std::make_unique<CreateTableNodeBuilder>();
    }
};