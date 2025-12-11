#include "query_executor.hpp"
#include "stg/row_validator.hpp"

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
        _storage_engine.Schemas().Write(
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

        // Load schema for validation
        auto schema = _storage_engine.Schemas().Read(database, node->table);
        if (!schema.has_value())
        {
            BOOST_LOG_TRIVIAL(error) << "Table '" << node->table << "' does not exist or has no schema.";
            return;
        }

        // Validate row against schema
        RowValidator validator(schema.value());
        auto validation = validator.Validate(node->columns, node->values);
        if (!validation.success)
        {
            BOOST_LOG_TRIVIAL(error) << "Validation error: " << validation.error_message;
            return;
        }

        // build data bytes from values
        std::string data_str;
        for (size_t i = 0; i < node->values.size(); i++)
        {
            data_str += node->values[i]->GetValue();
            if (i < node->values.size() - 1)
            {
                data_str += ",";
            }
        }

        // convert to bytes and insert via TableManager
        auto data = std::vector<uint8_t>(data_str.begin(), data_str.end());
        auto record_id = _storage_engine.Tables().Insert(database, node->table, data);

        if (!record_id)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to insert row into table '" << node->table << "'";
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "Inserted row into table '" << node->table
                                << "' at page " << record_id->page_id
                                << ", record " << record_id->record_index;
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