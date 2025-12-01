#ifndef __DROP_DATABASE_NODE_BUILDER_HPP__
#define __DROP_DATABASE_NODE_BUILDER_HPP__

#include "node_builder.hpp"

namespace litedb::query
{
    class DropDatabaseNodeBuilder : public NodeBuilder
    {
    public:
        DropDatabaseNodeBuilder();
        DropDatabaseNodeBuilder &SetDatabase(std::string &database);
        std::shared_ptr<AstNode> Build() override;
        std::unique_ptr<NodeBuilder> Clone() override;

    private:
        std::shared_ptr<DropDatabaseNode> _node;
    };
};

#endif // __DROP_DATABASE_NODE_BUILDER_HPP__