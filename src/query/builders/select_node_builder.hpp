#ifndef __SELECT_NODE_BUILDER_HPP__
#define __SELECT_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace litedb
{
    class SelectNodeBuilder : public NodeBuilder
    {
    public:
        SelectNodeBuilder();
        SelectNodeBuilder &AddColumn(std::string &column);
        SelectNodeBuilder &SetTable(std::string &table);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<SelectNode> _node;
    };
};

#endif // __SELECT_NODE_BUILDER_HPP__