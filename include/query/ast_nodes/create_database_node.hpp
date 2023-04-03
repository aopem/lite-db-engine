#ifndef __CREATE_DATABASE_NODE_HPP__
#define __CREATE_DATABASE_NODE_HPP__

#include "ast_node.hpp"

#include <string>
#include <memory>

namespace engine::query
{
    class CreateDatabaseNode : public AstNode, public std::enable_shared_from_this<CreateDatabaseNode>
    {
        public:
            std::string database;
    };
};

#endif // __CREATE_DATABASE_NODE_HPP__