#ifndef __DROP_DATABASE_NODE_HPP__
#define __DROP_DATABASE_NODE_HPP__

#include "ast_node.hpp"

namespace engine::query
{
    class DropDatabaseNode : public AstNode
    {
        public:
            std::string database;
    };
};

#endif // __DROP_DATABASE_NODE_HPP__