#include "update_node.hpp"

namespace engine::query
{
    void UpdateNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}