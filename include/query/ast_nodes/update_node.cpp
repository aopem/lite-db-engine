#include "update_node.hpp"

namespace litedb
{
    void UpdateNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}