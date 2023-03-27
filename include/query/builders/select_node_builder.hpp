#ifndef __SELECT_NODE_BUILDER_HPP__
#define __SELECT_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace engine::query
{
    class SelectNodeBuilder : public NodeBuilder
    {
        public:
            SelectNodeBuilder();
            SelectNodeBuilder& AddColumn(std::string& column);
            SelectNodeBuilder& SetTable(std::string& table);
            std::unique_ptr<AstNode> Build() override;

        private:
            std::unique_ptr<SelectNode> _node;
    };
};

#endif // __SELECT_NODE_BUILDER_HPP__