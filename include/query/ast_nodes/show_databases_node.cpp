#include "show_databases_node.hpp"

namespace engine::query
{
    void ShowDatabasesNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}