#include "stg/storage_engine.hpp"

#include <boost/log/trivial.hpp>

namespace litedb
{
    StorageEngine::StorageEngine() : _storage_data_dir(_DEFAULT_STORAGE_DIR)
    {
        // create storage data directory if it does not exist
        if (!std::filesystem::exists(_storage_data_dir))
        {
            std::filesystem::create_directories(_storage_data_dir);
            BOOST_LOG_TRIVIAL(debug) << "Created storage data directory at path: " << _storage_data_dir;
        }
    };

    std::vector<std::string> StorageEngine::ListDatabases()
    {
        std::vector<std::string> databases;

        BOOST_LOG_TRIVIAL(debug) << "Listing databases in data directory: " << _storage_data_dir;
        for (auto &entry : std::filesystem::directory_iterator(_storage_data_dir))
        {
            if (entry.is_directory())
            {
                databases.push_back(entry.path().filename().string());
                BOOST_LOG_TRIVIAL(debug) << "Found database: " << entry.path().filename().string();
            }
        }

        return databases;
    }

    std::vector<std::string> StorageEngine::ListTables(std::string_view database)
    {
        std::vector<std::string> tables;
        auto db_dir = _storage_data_dir / database;
        if (!std::filesystem::exists(db_dir) || !std::filesystem::is_directory(db_dir))
        {
            BOOST_LOG_TRIVIAL(warning) << "Database directory: " << db_dir << " does not exist.";
            return tables;
        }

        BOOST_LOG_TRIVIAL(debug) << "Listing tables in database directory: " << db_dir;
        for (auto &entry : std::filesystem::directory_iterator(db_dir))
        {
            if (entry.is_regular_file())
            {
                tables.push_back(entry.path().filename().string());
                BOOST_LOG_TRIVIAL(debug) << "Found table: " << entry.path().filename().string();
            }
        }

        return tables;
    }

    void StorageEngine::CreateDatabase(std::string_view database)
    {
        // add "database" to end of storage data dir
        auto db_dir = _storage_data_dir / database;
        if (!std::filesystem::exists(db_dir))
        {
            // create empty directory for database
            std::filesystem::create_directory(db_dir);
            BOOST_LOG_TRIVIAL(debug) << "Created storage file at path: " << db_dir;
        }
        else
        {
            BOOST_LOG_TRIVIAL(debug) << "Storage file at path: " << db_dir << " already exists, skipping creation.";
        }
    }

    void StorageEngine::CreateTable(std::string_view database, std::string_view table)
    {
        auto table_file = _storage_data_dir / database / table;

        // create empty file for table
        if (!std::filesystem::exists(table_file))
        {
            auto file = fopen(table_file.c_str(), "w");
            if (file == nullptr)
            {
                BOOST_LOG_TRIVIAL(error) << "Failed to create storage file at path: " << table_file;
                return;
            }

            fclose(file);
            BOOST_LOG_TRIVIAL(debug) << "Created storage file at path: " << table_file;
        }
        else
        {
            BOOST_LOG_TRIVIAL(debug) << "Storage file at path: " << table_file << " already exists, skipping creation.";
        }
    }

    void StorageEngine::WriteToTable(std::string_view database, std::string_view table, std::string_view data)
    {
        auto table_file = _storage_data_dir / database / table;

        // append data to storage data file
        if (std::filesystem::exists(table_file))
        {
            auto file = fopen(table_file.c_str(), "a");
            if (file == nullptr)
            {
                BOOST_LOG_TRIVIAL(error) << "Failed to open storage file at path: " << table_file << " for writing.";
                return;
            }

            fwrite(data.data(), sizeof(char), data.size(), file);
            fclose(file);
            BOOST_LOG_TRIVIAL(debug) << "Wrote data to storage file at path: " << table_file;
        }
        else
        {
            BOOST_LOG_TRIVIAL(warning) << "Storage file at path: " << table_file << " does not exist, cannot write data.";
        }
    }

    void StorageEngine::DeleteDatabase(std::string_view database)
    {
        auto db_dir = _storage_data_dir / database;

        // recursively delete a storage data file
        if (std::filesystem::exists(db_dir))
        {
            std::filesystem::remove(db_dir);
            BOOST_LOG_TRIVIAL(debug) << "Deleted storage file at path: " << db_dir;
        }
        else
        {
            BOOST_LOG_TRIVIAL(warning) << "Storage file at path: " << db_dir << " does not exist, cannot delete.";
        }
    }

    void StorageEngine::DeleteTable(std::string_view table)
    {
        auto table_file = _storage_data_dir / table;

        // delete a storage data file
        if (std::filesystem::exists(table_file))
        {
            std::filesystem::remove(table_file);
            BOOST_LOG_TRIVIAL(debug) << "Deleted storage file at path: " << table_file;
        }
        else
        {
            BOOST_LOG_TRIVIAL(warning) << "Storage file at path: " << table_file << " does not exist, cannot delete.";
        }
    }
}