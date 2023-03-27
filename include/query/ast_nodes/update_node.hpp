#ifndef __UPDATE_NODE_HPP__
#define __UPDATE_NODE_HPP__

#include "ast_node.hpp"

namespace engine::query
{
    class UpdateNode : public AstNode
    {
        public:
            UpdateNode();
    };
};

#endif // __UPDATE_NODE_HPP__