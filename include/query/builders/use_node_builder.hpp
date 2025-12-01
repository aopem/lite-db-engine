#ifndef __USE_NODE_BUILDER_HPP__
#define __USE_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../token.hpp"

#include <string>

namespace litedb
{
    class UseNodeBuilder : public NodeBuilder
    {
    public:
        UseNodeBuilder();
        UseNodeBuilder &SetDatabase(std::string &database);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<UseNode> _node;
    };
};

#endif // __USE_NODE_BUILDER_HPP__