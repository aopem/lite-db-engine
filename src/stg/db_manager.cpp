#include "stg/db_manager.hpp"

#include <boost/log/trivial.hpp>

namespace litedb
{
    DbManager::DbManager(std::filesystem::path base_dir)
        : _base_dir(base_dir) {}

    std::vector<std::string> DbManager::List()
    {
        std::vector<std::string> databases;

        BOOST_LOG_TRIVIAL(debug) << "Listing databases in data directory: " << _base_dir;
        for (auto &entry : std::filesystem::directory_iterator(_base_dir))
        {
            if (!entry.is_directory())
            {
                continue;
            }

            databases.push_back(entry.path().filename().string());
            BOOST_LOG_TRIVIAL(debug) << "Found database: " << entry.path().filename().string();
        }

        return databases;
    }

    void DbManager::Create(std::string_view database)
    {
        auto db_dir = _base_dir / database;
        if (std::filesystem::exists(db_dir))
        {
            BOOST_LOG_TRIVIAL(debug) << "Database at path: " << db_dir << " already exists, skipping creation.";
            return;
        }

        std::filesystem::create_directory(db_dir);
        BOOST_LOG_TRIVIAL(debug) << "Created database at path: " << db_dir;
    }

    void DbManager::Delete(std::string_view database)
    {
        auto db_dir = _base_dir / database;
        if (!std::filesystem::exists(db_dir))
        {
            BOOST_LOG_TRIVIAL(warning) << "Database at path: " << db_dir << " does not exist, cannot delete.";
            return;
        }

        std::filesystem::remove_all(db_dir);
        BOOST_LOG_TRIVIAL(debug) << "Deleted database at path: " << db_dir;
    }

    bool DbManager::Exists(std::string_view database)
    {
        auto db_dir = _base_dir / database;
        return std::filesystem::exists(db_dir) && std::filesystem::is_directory(db_dir);
    }
}
