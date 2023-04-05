#ifndef __SHOW_DATABASES_NODE_BUILDER_HPP__
#define __SHOW_DATABASES_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../token.hpp"

#include <string>

namespace litedb
{
    class ShowDatabasesNodeBuilder : public NodeBuilder
    {
        public:
            ShowDatabasesNodeBuilder();
            std::shared_ptr<AstNode> Build() override;
            std::unique_ptr<NodeBuilder> Clone() override;

        private:
            std::shared_ptr<ShowDatabasesNode> _node;
    };
};

#endif // __SHOW_DATABASES_NODE_BUILDER_HPP__