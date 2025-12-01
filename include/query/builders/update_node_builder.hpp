#ifndef __UPDATE_NODE_BUILDER_HPP__
#define __UPDATE_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../token.hpp"

#include <string>

namespace litedb::query
{
    class UpdateNodeBuilder : public NodeBuilder
    {
    public:
        UpdateNodeBuilder();
        UpdateNodeBuilder &SetTable(std::string &table);
        UpdateNodeBuilder &AddUpdatedEntry(std::pair<std::string, std::shared_ptr<Token>> entry);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<UpdateNode> _node;
    };
};

#endif // __UPDATE_NODE_BUILDER_HPP__