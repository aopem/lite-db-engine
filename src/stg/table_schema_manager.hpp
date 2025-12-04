#ifndef __TABLE_SCHEMA_MANAGER_HPP__
#define __TABLE_SCHEMA_MANAGER_HPP__

#include "stg/table_schema.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace litedb
{
    class TableSchemaManager
    {
    public:
        TableSchemaManager(std::filesystem::path base_dir);
        ~TableSchemaManager() = default;

        void Write(std::string_view database, std::string_view table, const TableSchema &schema);
        std::optional<TableSchema> Read(std::string_view database, std::string_view table) const;
        bool Exists(std::string_view database, std::string_view table) const;
        void Delete(std::string_view database, std::string_view table);

    private:
        std::filesystem::path _base_dir;

        std::filesystem::path GetSchemaPath(std::string_view database, std::string_view table) const;
    };
}

#endif // __TABLE_SCHEMA_MANAGER_HPP__
