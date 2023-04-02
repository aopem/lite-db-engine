#ifndef __DELETE_NODE_HPP__
#define __DELETE_NODE_HPP__

#include "ast_node.hpp"

#include <string>

namespace engine::query
{
    class DeleteNode : public AstNode
    {
        public:
            std::string table;
    };
};

#endif // __DELETE_NODE_HPP__