#include "create_database_node.hpp"

namespace litedb
{
    void CreateDatabaseNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}