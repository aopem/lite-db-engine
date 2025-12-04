#ifndef __TABLE_SCHEMA_HPP__
#define __TABLE_SCHEMA_HPP__

#include "query/data.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace litedb
{
    struct Column
    {
        std::string name;
        data_type_t type;
    };

    class TableSchema
    {
    public:
        std::vector<Column> columns;

        TableSchema() = default;
        ~TableSchema() = default;

        const data_type_t *GetColumnType(const std::string &name) const
        {
            auto it = _column_map.find(name);
            if (it != _column_map.end())
            {
                return &it->second;
            }

            return nullptr;
        }

        bool HasColumn(const std::string &name) const
        {
            return _column_map.find(name) != _column_map.end();
        }

        void AddColumn(const std::string &name, data_type_t type)
        {
            columns.emplace_back(Column{name, type});
            _column_map[name] = type;
        }

    private:
        std::unordered_map<std::string, data_type_t> _column_map;
    };
}

#endif // __TABLE_SCHEMA_HPP__
