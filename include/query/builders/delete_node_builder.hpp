#ifndef __DELETE_NODE_BUILDER_HPP__
#define __DELETE_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace engine::query
{
    class DeleteNodeBuilder : public NodeBuilder
    {
        public:
            DeleteNodeBuilder();
            DeleteNodeBuilder& SetTable(std::string& table);
            std::shared_ptr<AstNode> Build() override;
            std::unique_ptr<NodeBuilder> Clone() override;

        private:
            std::shared_ptr<DeleteNode> _node;
    };
};

#endif // __DELETE_NODE_BUILDER_HPP__