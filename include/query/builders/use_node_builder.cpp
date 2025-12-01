#include "use_node_builder.hpp"

namespace litedb
{
    UseNodeBuilder::UseNodeBuilder()
    {
        _node = std::make_shared<UseNode>();
    }

    UseNodeBuilder &UseNodeBuilder::SetDatabase(std::string &database)
    {
        _node->database = database;
        return *this;
    }

    std::shared_ptr<AstNode> UseNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> UseNodeBuilder::Clone()
    {
        return std::make_unique<UseNodeBuilder>();
    }
};