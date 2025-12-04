#include "delete_node.hpp"

namespace litedb
{
    void DeleteNode::Accept(const std::shared_ptr<AstNodeVisitor> &visitor)
    {
        visitor->Visit(shared_from_this());
    }
}