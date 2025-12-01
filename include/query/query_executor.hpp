#ifndef __QUERY_EXECUTOR_HPP__
#define __QUERY_EXECUTOR_HPP__

#include "stg/storage_engine.hpp"
#include "query/ast_node_visitor.hpp"

#include <memory>

#include <boost/log/trivial.hpp>

namespace litedb
{
    class QueryExecutor : public AstNodeVisitor
    {
    public:
        virtual void Visit(std::shared_ptr<CreateDatabaseNode> node) override;
        virtual void Visit(std::shared_ptr<CreateTableNode> node) override;
        virtual void Visit(std::shared_ptr<DeleteNode> node) override;
        virtual void Visit(std::shared_ptr<DropDatabaseNode> node) override;
        virtual void Visit(std::shared_ptr<SelectNode> node) override;
        virtual void Visit(std::shared_ptr<UpdateNode> node) override;
        virtual void Visit(std::shared_ptr<UseNode> node) override;
        virtual void Visit(std::shared_ptr<ShowDatabasesNode> node) override;

    private:
        std::unique_ptr<StorageEngine> _storage_engine = std::make_unique<StorageEngine>();
    };
};

#endif // __QUERY_EXECUTOR_HPP__