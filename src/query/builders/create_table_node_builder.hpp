#ifndef __CREATE_TABLE_NODE_BUILDER_HPP__
#define __CREATE_TABLE_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../data.hpp"

namespace litedb
{
    class CreateTableNodeBuilder : public NodeBuilder
    {
    public:
        CreateTableNodeBuilder();
        CreateTableNodeBuilder &SetDatabase(const std::string &database);
        CreateTableNodeBuilder &SetTable(const std::string &database);
        CreateTableNodeBuilder &AddColumn(const std::string &column, data_type_t data_type);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<CreateTableNode> _node;
    };
};

#endif // __CREATE_TABLE_NODE_BUILDER_HPP__