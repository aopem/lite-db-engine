#include "insert_into_node.hpp"

namespace litedb
{
    void InsertIntoNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}
