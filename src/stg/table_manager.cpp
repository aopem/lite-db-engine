#include "stg/table_manager.hpp"

#include <boost/log/trivial.hpp>

namespace litedb
{
    TableManager::TableManager(std::filesystem::path base_dir)
        : _base_dir(base_dir) {}

    std::vector<std::string> TableManager::List(std::string_view database) const
    {
        std::vector<std::string> tables;
        auto db_dir = _base_dir / database;
        if (!std::filesystem::exists(db_dir) || !std::filesystem::is_directory(db_dir))
        {
            BOOST_LOG_TRIVIAL(warning) << "Database directory: " << db_dir << " does not exist.";
            return tables;
        }

        BOOST_LOG_TRIVIAL(debug) << "Listing tables in database directory: " << db_dir;
        for (auto &entry : std::filesystem::directory_iterator(db_dir))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            tables.push_back(entry.path().filename().string());
            BOOST_LOG_TRIVIAL(debug) << "Found table: " << entry.path().filename().string();
        }

        return tables;
    }

    void TableManager::Create(std::string_view database, std::string_view table)
    {
        auto table_file = _base_dir / database / table;
        if (std::filesystem::exists(table_file))
        {
            BOOST_LOG_TRIVIAL(debug) << "Table at path: " << table_file << " already exists, skipping creation.";
            return;
        }

        auto file = fopen(table_file.c_str(), "w");
        if (file == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to create table at path: " << table_file;
            return;
        }

        fclose(file);
        BOOST_LOG_TRIVIAL(debug) << "Created table at path: " << table_file;
    }

    void TableManager::Delete(std::string_view database, std::string_view table)
    {
        auto table_file = _base_dir / database / table;
        if (!std::filesystem::exists(table_file))
        {
            BOOST_LOG_TRIVIAL(warning) << "Table at path: " << table_file << " does not exist, cannot delete.";
            return;
        }

        std::filesystem::remove(table_file);
        BOOST_LOG_TRIVIAL(debug) << "Deleted table at path: " << table_file;
    }

    bool TableManager::Exists(std::string_view database, std::string_view table) const
    {
        auto table_file = _base_dir / database / table;
        return std::filesystem::exists(table_file) && std::filesystem::is_regular_file(table_file);
    }
}
