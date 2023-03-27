#ifndef __CREATE_TABLE_NODE_HPP__
#define __CREATE_TABLE_NODE_HPP__

#include "ast_node.hpp"

namespace engine::query
{
    class CreateTableNode : public AstNode
    {
        public:
            CreateTableNode();
    };
};

#endif // __CREATE_TABLE_NODE_HPP__