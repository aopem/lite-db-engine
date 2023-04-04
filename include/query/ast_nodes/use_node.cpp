#include "use_node.hpp"

namespace engine::query
{
    void UseNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}