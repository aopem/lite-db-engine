#ifndef __NODE_BUILDER_HPP__
#define __NODE_BUILDER_HPP__

#include "../ast_nodes/ast_nodes.hpp"

#include <memory>

namespace litedb
{
    class NodeBuilder
    {
        public:
            NodeBuilder() = default;
            virtual ~NodeBuilder() = default;
            virtual std::shared_ptr<AstNode> Build() = 0;
            virtual std::unique_ptr<NodeBuilder> Clone() = 0;
    };
};

#endif // __NODE_BUILDER_HPP__