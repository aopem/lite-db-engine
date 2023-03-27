#ifndef __CREATE_DATABASE_NODE_HPP__
#define __CREATE_DATABASE_NODE_HPP__

#include "ast_node.hpp"

#include <string>

namespace engine::query
{
    class CreateDatabaseNode : public AstNode
    {
        public:
            std::string database;
    };
};

#endif // __CREATE_DATABASE_NODE_HPP__