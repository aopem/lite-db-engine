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
            std::unique_ptr<AstNode> Build() override;

        private:
            std::unique_ptr<DeleteNode> _node;
    };
};

#endif // __DELETE_NODE_BUILDER_HPP__