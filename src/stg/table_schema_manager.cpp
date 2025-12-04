#include "stg/table_schema_manager.hpp"

#include <boost/log/trivial.hpp>
#include <cstdio>

namespace litedb
{
    TableSchemaManager::TableSchemaManager(std::filesystem::path base_dir)
        : _base_dir(base_dir) {}

    std::filesystem::path TableSchemaManager::GetSchemaPath(std::string_view database, std::string_view table) const
    {
        return _base_dir / database / (std::string(table) + ".schema");
    }

    void TableSchemaManager::Write(std::string_view database, std::string_view table, const TableSchema &schema)
    {
        auto schema_file = GetSchemaPath(database, table);
        if (std::filesystem::exists(schema_file))
        {
            BOOST_LOG_TRIVIAL(debug) << "Schema file at path: " << schema_file << " already exists, skipping creation.";
            return;
        }

        auto file = fopen(schema_file.c_str(), "w");
        if (file == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to create schema file at path: " << schema_file;
            return;
        }

        for (const auto &column : schema.columns)
        {
            fprintf(file, "%s,%d,%zu\n", column.name.c_str(),
                    static_cast<int>(column.type.name), column.type.length);
        }

        fclose(file);
        BOOST_LOG_TRIVIAL(debug) << "Created schema file at path: " << schema_file;
    }

    std::optional<TableSchema> TableSchemaManager::Read(std::string_view database, std::string_view table)
    {
        auto schema_file = GetSchemaPath(database, table);
        if (!std::filesystem::exists(schema_file))
        {
            BOOST_LOG_TRIVIAL(warning) << "Schema file not found: " << schema_file;
            return std::nullopt;
        }

        auto file = fopen(schema_file.c_str(), "r");
        if (file == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to open schema file: " << schema_file;
            return std::nullopt;
        }

        TableSchema schema;
        char buffer[256];
        char column_name[128];
        int data_type_int;
        size_t length;
        while (fgets(buffer, sizeof(buffer), file))
        {
            // read up to 127 chars for name, int for type, size_t for length
            if (sscanf(buffer, "%127[^,],%d,%zu", column_name, &data_type_int, &length) == 3)
            {
                data_type_t data_type;
                data_type.name = static_cast<data_type_e>(data_type_int);
                data_type.length = length;
                schema.AddColumn(column_name, data_type);
            }
        }

        fclose(file);
        BOOST_LOG_TRIVIAL(debug) << "Loaded schema for table: " << table
                                 << " with " << schema.columns.size() << " columns";
        return schema;
    }

    bool TableSchemaManager::Exists(std::string_view database, std::string_view table)
    {
        auto schema_file = GetSchemaPath(database, table);
        return std::filesystem::exists(schema_file) && std::filesystem::is_regular_file(schema_file);
    }

    void TableSchemaManager::Delete(std::string_view database, std::string_view table)
    {
        auto schema_file = GetSchemaPath(database, table);
        if (!std::filesystem::exists(schema_file))
        {
            BOOST_LOG_TRIVIAL(warning) << "Schema file at path: " << schema_file << " does not exist, cannot delete.";
            return;
        }

        std::filesystem::remove(schema_file);
        BOOST_LOG_TRIVIAL(debug) << "Deleted schema file at path: " << schema_file;
    }
}
