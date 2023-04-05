#include "show_databases_node_builder.hpp"

namespace litedb
{
    ShowDatabasesNodeBuilder::ShowDatabasesNodeBuilder()
    {
        _node = std::make_shared<ShowDatabasesNode>();
    }

    std::shared_ptr<AstNode> ShowDatabasesNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> ShowDatabasesNodeBuilder::Clone()
    {
        return std::make_unique<ShowDatabasesNodeBuilder>();
    }
};