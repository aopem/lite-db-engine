#include "use_node.hpp"

namespace litedb
{
    void UseNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}