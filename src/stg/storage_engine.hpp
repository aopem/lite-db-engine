#ifndef __STORAGE_ENGINE_HPP__
#define __STORAGE_ENGINE_HPP__

#include "stg/db_manager.hpp"
#include "stg/table_manager.hpp"
#include "stg/table_schema_manager.hpp"

#include <filesystem>
#include <memory>
#include <string_view>

namespace litedb
{
    class StorageEngine
    {
    public:
        StorageEngine();
        ~StorageEngine() = default;

        DbManager &Databases() { return *_db_manager; }
        TableManager &Tables() { return *_table_manager; }
        TableSchemaManager &Schemas() { return *_table_schema_manager; }

    private:
        static constexpr std::string_view _DEFAULT_STORAGE_DIR = "./.litedb/data/";
        std::filesystem::path _storage_data_dir;

        std::unique_ptr<DbManager> _db_manager;
        std::unique_ptr<TableManager> _table_manager;
        std::unique_ptr<TableSchemaManager> _table_schema_manager;
    };
};

#endif // __STORAGE_ENGINE_HPP__