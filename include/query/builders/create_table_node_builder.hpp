#ifndef __CREATE_TABLE_NODE_BUILDER_HPP__
#define __CREATE_TABLE_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../data.hpp"

namespace litedb::query
{
    class CreateTableNodeBuilder : public NodeBuilder
    {
    public:
        CreateTableNodeBuilder();
        CreateTableNodeBuilder &SetDatabase(std::string &database);
        CreateTableNodeBuilder &SetTable(std::string &database);
        CreateTableNodeBuilder &AddColumn(std::pair<std::string, data_type_t> column);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<CreateTableNode> _node;
    };
};

#endif // __CREATE_TABLE_NODE_BUILDER_HPP__