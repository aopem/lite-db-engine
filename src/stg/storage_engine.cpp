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

        // initialize managers
        _db_manager = std::make_unique<DbManager>(_storage_data_dir);
        _table_manager = std::make_unique<TableManager>(_storage_data_dir);
        _table_schema_manager = std::make_unique<TableSchemaManager>(_storage_data_dir);
    }
}