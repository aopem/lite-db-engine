#include "delete_node.hpp"

namespace litedb::query
{
    void DeleteNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}