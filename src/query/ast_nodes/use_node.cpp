#include "use_node.hpp"

namespace litedb
{
    void UseNode::Accept(const std::shared_ptr<AstNodeVisitor> &visitor)
    {
        visitor->Visit(shared_from_this());
    }
}