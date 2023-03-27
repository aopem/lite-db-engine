#ifndef __DELETE_NODE_HPP__
#define __DELETE_NODE_HPP__

#include "ast_node.hpp"

namespace engine::query
{
    class DeleteNode : public AstNode
    {
        public:
            DeleteNode();
    };
};

#endif // __DELETE_NODE_HPP__