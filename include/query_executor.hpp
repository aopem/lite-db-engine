#ifndef __QUERY_EXECUTOR_HPP__
#define __QUERY_EXECUTOR_HPP__

#include "storage/storage_engine.hpp"
#include "query/ast_node_visitor.hpp"

#include <memory>

#include <boost/log/trivial.hpp>

namespace litedb
{
    class QueryExecutor : public query::AstNodeVisitor
    {
    public:
        virtual void Visit(std::shared_ptr<query::CreateDatabaseNode> node) override;
        virtual void Visit(std::shared_ptr<query::CreateTableNode> node) override;
        virtual void Visit(std::shared_ptr<query::DeleteNode> node) override;
        virtual void Visit(std::shared_ptr<query::DropDatabaseNode> node) override;
        virtual void Visit(std::shared_ptr<query::SelectNode> node) override;
        virtual void Visit(std::shared_ptr<query::UpdateNode> node) override;
        virtual void Visit(std::shared_ptr<query::UseNode> node) override;
        virtual void Visit(std::shared_ptr<query::ShowDatabasesNode> node) override;

    private:
        std::unique_ptr<StorageEngine> _storage_engine = std::make_unique<StorageEngine>();
    };
};

#endif // __QUERY_EXECUTOR_HPP__