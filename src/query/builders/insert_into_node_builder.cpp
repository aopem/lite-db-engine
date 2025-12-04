#include "insert_into_node_builder.hpp"

namespace litedb
{
    InsertIntoNodeBuilder::InsertIntoNodeBuilder()
    {
        _node = std::make_shared<InsertIntoNode>();
    }

    InsertIntoNodeBuilder &InsertIntoNodeBuilder::SetTable(const std::string &table)
    {
        _node->table = table;
        return *this;
    }

    InsertIntoNodeBuilder &InsertIntoNodeBuilder::AddColumn(const std::string &column)
    {
        _node->columns.push_back(column);
        return *this;
    }

    InsertIntoNodeBuilder &InsertIntoNodeBuilder::AddValue(const std::shared_ptr<Token> &value)
    {
        _node->values.push_back(value);
        return *this;
    }

    std::shared_ptr<AstNode> InsertIntoNodeBuilder::Build()
    {
        return _node;
    }

    std::unique_ptr<NodeBuilder> InsertIntoNodeBuilder::Clone()
    {
        return std::make_unique<InsertIntoNodeBuilder>();
    }
};
