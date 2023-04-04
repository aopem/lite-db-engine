#include "drop_database_node.hpp"

namespace engine::query
{
    void DropDatabaseNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}