#include "create_table_node_builder.hpp"

namespace litedb
{
    CreateTableNodeBuilder::CreateTableNodeBuilder()
    {
        _node = std::make_shared<CreateTableNode>();
    }

    CreateTableNodeBuilder &CreateTableNodeBuilder::SetDatabase(std::string &database)
    {
        _node->database = database;
        return *this;
    }

    CreateTableNodeBuilder &CreateTableNodeBuilder::SetTable(std::string &table)
    {
        _node->table = table;
        return *this;
    }

    CreateTableNodeBuilder &CreateTableNodeBuilder::AddColumn(std::string &column, data_type_t data_type)
    {
        _node->schema.columns[column] = data_type;
        return *this;
    }

    std::shared_ptr<AstNode> CreateTableNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> CreateTableNodeBuilder::Clone()
    {
        return std::make_unique<CreateTableNodeBuilder>();
    }
};