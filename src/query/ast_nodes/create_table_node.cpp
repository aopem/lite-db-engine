#include "create_table_node.hpp"

namespace litedb
{
    void CreateTableNode::Accept(std::shared_ptr<AstNodeVisitor> visitor)
    {
        visitor->Visit(shared_from_this());
    }
}