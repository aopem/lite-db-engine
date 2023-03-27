#include "create_database_node_builder.hpp"

namespace engine::query
{
    CreateDatabaseNodeBuilder::CreateDatabaseNodeBuilder()
    {
        _node = std::make_unique<CreateDatabaseNode>();
    }

    CreateDatabaseNodeBuilder& CreateDatabaseNodeBuilder::SetDatabase(std::string& database)
    {
        _node->database = database;
        return *this;
    }

    std::unique_ptr<AstNode> CreateDatabaseNodeBuilder::Build()
    {
        return std::move(_node);
    }
};