#include "create_database_node_builder.hpp"

namespace litedb::query
{
    CreateDatabaseNodeBuilder::CreateDatabaseNodeBuilder()
    {
        _node = std::make_shared<CreateDatabaseNode>();
    }

    CreateDatabaseNodeBuilder &CreateDatabaseNodeBuilder::SetDatabase(std::string &database)
    {
        _node->database = database;
        return *this;
    }

    std::shared_ptr<AstNode> CreateDatabaseNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> CreateDatabaseNodeBuilder::Clone()
    {
        return std::make_unique<CreateDatabaseNodeBuilder>();
    }
};