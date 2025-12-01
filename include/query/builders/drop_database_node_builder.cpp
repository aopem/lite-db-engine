#include "drop_database_node_builder.hpp"

namespace litedb::query
{
    DropDatabaseNodeBuilder::DropDatabaseNodeBuilder()
    {
        _node = std::make_shared<DropDatabaseNode>();
    }

    DropDatabaseNodeBuilder &DropDatabaseNodeBuilder::SetDatabase(std::string &database)
    {
        _node->database = database;
        return *this;
    }

    std::shared_ptr<AstNode> DropDatabaseNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> DropDatabaseNodeBuilder::Clone()
    {
        return std::make_unique<DropDatabaseNodeBuilder>();
    }
};