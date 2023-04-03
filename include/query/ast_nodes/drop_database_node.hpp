#ifndef __DROP_DATABASE_NODE_HPP__
#define __DROP_DATABASE_NODE_HPP__

#include "ast_node.hpp"

#include <string>
#include <memory>

namespace engine::query
{
    class DropDatabaseNode : public AstNode, public std::enable_shared_from_this<DropDatabaseNode>
    {
        public:
            std::string database;
    };
};

#endif // __DROP_DATABASE_NODE_HPP__