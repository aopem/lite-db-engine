#include "select_node.hpp"

namespace engine::query
{
    void SelectNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}