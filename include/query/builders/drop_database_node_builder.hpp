#ifndef __DROP_DATABASE_NODE_BUILDER_HPP__
#define __DROP_DATABASE_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace engine::query
{
    class DropDatabaseNodeBuilder : public NodeBuilder
    {
        public:
            DropDatabaseNodeBuilder();
            DropDatabaseNodeBuilder& SetDatabase(std::string& database);
            std::unique_ptr<AstNode> Build() override;

        private:
            std::unique_ptr<DropDatabaseNode> _node;
    };
};

#endif // __DROP_DATABASE_NODE_BUILDER_HPP__