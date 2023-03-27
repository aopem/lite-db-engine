#ifndef __CREATE_TABLE_NODE_BUILDER_HPP__
#define __CREATE_TABLE_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace engine::query
{
    class CreateTableNodeBuilder : public NodeBuilder
    {
        public:
            CreateTableNodeBuilder();
            CreateTableNodeBuilder& SetDatabase(std::string& database);
            CreateTableNodeBuilder& SetTable(std::string& database);
            CreateTableNodeBuilder& AddColumn(std::pair<std::string, std::string> column);
            std::unique_ptr<AstNode> Build() override;

        private:
            std::unique_ptr<CreateTableNode> _node;
    };
};

#endif // __CREATE_TABLE_NODE_BUILDER_HPP__