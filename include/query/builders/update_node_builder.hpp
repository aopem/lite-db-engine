#ifndef __UPDATE_NODE_BUILDER_HPP__
#define __UPDATE_NODE_BUILDER_HPP__

#include "node_builder.hpp"
#include "../token.hpp"

#include <string>

namespace engine::query
{
    class UpdateNodeBuilder : public NodeBuilder
    {
        public:
            UpdateNodeBuilder();
            UpdateNodeBuilder& SetTable(std::string& table);
            UpdateNodeBuilder& AddUpdatedEntry(std::pair<std::string, std::shared_ptr<Token>> entry);
            std::unique_ptr<AstNode> Build() override;

        private:
            std::unique_ptr<UpdateNode> _node;
    };
};

#endif // __UPDATE_NODE_BUILDER_HPP__