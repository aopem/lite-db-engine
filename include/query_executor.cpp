#include "query_executor.hpp"

namespace litedb
{
    void QueryExecutor::Visit(std::shared_ptr<CreateDatabaseNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing CREATE DATABASE";
    }

    void QueryExecutor::Visit(std::shared_ptr<CreateTableNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing CREATE TABLE";
    }

    void QueryExecutor::Visit(std::shared_ptr<DeleteNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing DELETE";
    }

    void QueryExecutor::Visit(std::shared_ptr<DropDatabaseNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing DROP DATABASE";
    }

    void QueryExecutor::Visit(std::shared_ptr<SelectNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing SELECT";
    }

    void QueryExecutor::Visit(std::shared_ptr<UpdateNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing UPDATE";
    }

    void QueryExecutor::Visit(std::shared_ptr<UseNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing USE";
    }

    void QueryExecutor::Visit(std::shared_ptr<ShowDatabasesNode> node)
    {
        BOOST_LOG_TRIVIAL(info) << "Executing SHOW DATABASES";
    }
}