#ifndef __CREATE_TABLE_NODE_HPP__
#define __CREATE_TABLE_NODE_HPP__

#include "ast_node.hpp"

#include <string>
#include <vector>

namespace engine::query
{
    class CreateTableNode : public AstNode
    {
        public:
            std::string database;
            std::string table;
            std::vector<std::pair<std::string, std::string>> columns;
    };
};

#endif // __CREATE_TABLE_NODE_HPP__