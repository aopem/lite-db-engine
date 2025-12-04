#include "insert_into_node.hpp"

namespace litedb
{
    void InsertIntoNode::Accept(const std::shared_ptr<AstNodeVisitor> &visitor)
    {
        visitor->Visit(shared_from_this());
    }
}
