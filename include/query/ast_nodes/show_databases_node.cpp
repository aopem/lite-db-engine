#include "show_databases_node.hpp"

namespace litedb::query
{
    void ShowDatabasesNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}