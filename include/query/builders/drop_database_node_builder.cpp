#include "drop_database_node_builder.hpp"

namespace engine::query
{
    DropDatabaseNodeBuilder::DropDatabaseNodeBuilder()
    {
        _node = std::make_unique<DropDatabaseNode>();
    }

    DropDatabaseNodeBuilder& DropDatabaseNodeBuilder::SetDatabase(std::string& database)
    {
        _node->database = database;
        return *this;
    }

    std::unique_ptr<AstNode> DropDatabaseNodeBuilder::Build()
    {
        return std::move(_node);
    }

    std::unique_ptr<NodeBuilder> DropDatabaseNodeBuilder::Clone()
    {
        return std::make_unique<DropDatabaseNodeBuilder>();
    }
};