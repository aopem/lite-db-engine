#include "create_table_node.hpp"

namespace engine::query
{
    void CreateTableNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}