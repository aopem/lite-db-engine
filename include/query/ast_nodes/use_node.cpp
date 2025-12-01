#include "use_node.hpp"

namespace litedb::query
{
    void UseNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}