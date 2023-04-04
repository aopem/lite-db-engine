#include "delete_node.hpp"

namespace engine::query
{
    void DeleteNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}