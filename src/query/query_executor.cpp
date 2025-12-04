#include "query_executor.hpp"

namespace litedb
{
    void QueryExecutor::Visit(std::shared_ptr<CreateDatabaseNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing CREATE DATABASE";
        _storage_engine.Databases().Create(node->database);
    }

    void QueryExecutor::Visit(std::shared_ptr<CreateTableNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing CREATE TABLE";
        _storage_engine.Tables().Create(_db_context.GetCurrentDatabase(), node->table);
        _storage_engine.Tables().CreateSchema(
            _db_context.GetCurrentDatabase(),
            node->table,
            node->schema);
    }

    void QueryExecutor::Visit(std::shared_ptr<DeleteNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing DELETE";
    }

    void QueryExecutor::Visit(std::shared_ptr<DropDatabaseNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing DROP DATABASE";
        _storage_engine.Databases().Delete(node->database);
    }

    void QueryExecutor::Visit(std::shared_ptr<InsertIntoNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing INSERT INTO";

        std::string database = std::string(_db_context.GetCurrentDatabase());
        if (database.empty())
        {
            BOOST_LOG_TRIVIAL(error) << "No database selected. Use 'USE <database>' first.";
            return;
        }

        // Build data string from values
        std::string data = "";
        for (size_t i = 0; i < node->values.size(); i++)
        {
            data += node->values[i]->GetValue();
            if (i < node->values.size() - 1)
            {
                data += ",";
            }
        }
        data += "\n";

        _storage_engine.Data().Write(database, node->table, data);
    }

    void QueryExecutor::Visit(std::shared_ptr<SelectNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing SELECT";
    }

    void QueryExecutor::Visit(std::shared_ptr<UpdateNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing UPDATE";
    }

    void QueryExecutor::Visit(std::shared_ptr<UseNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing USE";
        _db_context.SetCurrentDatabase(node->database);
    }

    void QueryExecutor::Visit(std::shared_ptr<ShowDatabasesNode> node)
    {
        BOOST_LOG_TRIVIAL(debug) << "Executing SHOW DATABASES";
        auto databases = _storage_engine.Databases().List();
        if (databases.empty())
        {
            BOOST_LOG_TRIVIAL(info) << "No databases found.";
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "Databases:";
        for (const auto &db : databases)
        {
            BOOST_LOG_TRIVIAL(info) << " - " << db;
        }
    }
}