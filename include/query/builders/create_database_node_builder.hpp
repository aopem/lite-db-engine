#ifndef __CREATE_DATABASE_NODE_BUILDER_HPP__
#define __CREATE_DATABASE_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace engine::query
{
    class CreateDatabaseNodeBuilder : public NodeBuilder
    {
        public:
            CreateDatabaseNodeBuilder();
            CreateDatabaseNodeBuilder& SetDatabase(std::string& database);
            std::unique_ptr<AstNode> Build() override;

        private:
            std::unique_ptr<CreateDatabaseNode> _node;
    };
};

#endif // __CREATE_DATABASE_NODE_BUILDER_HPP__