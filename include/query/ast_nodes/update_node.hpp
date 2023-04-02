#ifndef __UPDATE_NODE_HPP__
#define __UPDATE_NODE_HPP__

#include "ast_node.hpp"
#include "../token.hpp"

#include <string>

namespace engine::query
{
    class UpdateNode : public AstNode
    {
        public:
            std::string table;
            std::vector<std::pair<std::string, std::shared_ptr<Token>>> entries;
    };
};

#endif // __UPDATE_NODE_HPP__