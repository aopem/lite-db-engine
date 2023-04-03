#ifndef __NODE_BUILDER_HPP__
#define __NODE_BUILDER_HPP__

#include "../ast_nodes/ast_nodes.hpp"

#include <memory>

namespace engine::query
{
    class NodeBuilder
    {
        public:
            NodeBuilder() = default;
            virtual ~NodeBuilder() = default;
            virtual std::unique_ptr<AstNode> Build() = 0;
            virtual std::unique_ptr<NodeBuilder> Clone() = 0;
    };
};

#endif // __NODE_BUILDER_HPP__