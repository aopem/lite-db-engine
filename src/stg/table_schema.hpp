#ifndef __TABLE_SCHEMA_HPP__
#define __TABLE_SCHEMA_HPP__

#include "query/data.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace litedb
{
    class TableSchema
    {
    public:
        std::unordered_map<std::string, data_type_t> columns;

        TableSchema() = default;
        ~TableSchema() = default;
    };
}

#endif // __TABLE_SCHEMA_HPP__
