#ifndef __SELECT_NODE_HPP__
#define __SELECT_NODE_HPP__

#include "ast_node.hpp"

#include <vector>
#include <string>
#include <memory>

namespace engine::query
{
    class SelectNode : public AstNode, public std::enable_shared_from_this<SelectNode>
    {
        public:
            std::string table;
            std::vector<std::string> columns;
    };
};

#endif // __SELECT_NODE_HPP__