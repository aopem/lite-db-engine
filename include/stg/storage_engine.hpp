#ifndef __STORAGE_ENGINE_HPP__
#define __STORAGE_ENGINE_HPP__

#include <filesystem>
#include <string_view>

namespace litedb::stg
{
    class StorageEngine
    {
    public:
        StorageEngine();
        ~StorageEngine() = default;

        void CreateDatabase(std::string_view database);
        void CreateTable(std::string_view database, std::string_view table);
        void WriteToTable(std::string_view table, std::string_view data);
        void DeleteDatabase(std::string_view database);
        void DeleteTable(std::string_view table);

    private:
        static constexpr std::string_view _DEFAULT_STORAGE_DIR = "./.litedb/data/";
        std::filesystem::path _storage_data_dir;
    };
};

#endif // __STORAGE_ENGINE_HPP__