#ifndef __TABLE_MANAGER_HPP__
#define __TABLE_MANAGER_HPP__

#include "stg/table_schema.hpp"

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace litedb
{
    class TableManager
    {
    public:
        TableManager(std::filesystem::path base_dir);
        ~TableManager() = default;

        std::vector<std::string> List(std::string_view database);
        void Create(std::string_view database, std::string_view table);
        void CreateSchema(std::string_view database, std::string &table, TableSchema &schema);
        void Delete(std::string_view database, std::string_view table);
        bool Exists(std::string_view database, std::string_view table);

    private:
        std::filesystem::path _base_dir;
    };
}

#endif // __TABLE_MANAGER_HPP__
