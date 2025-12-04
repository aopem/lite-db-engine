#ifndef __DATA_TYPE_HPP__
#define __DATA_TYPE_HPP__

#include <string_view>

namespace litedb
{
    enum class data_type_e
    {
        INT,
        FLOAT,
        CHAR
    };

    struct data_type_t
    {
        data_type_e name;
        size_t length;
    };

    // Efficient constexpr function for enum to string (no heap allocation)
    constexpr std::string_view DataTypeToString(data_type_e type)
    {
        switch (type)
        {
        case data_type_e::INT:
            return "INT";
        case data_type_e::FLOAT:
            return "FLOAT";
        case data_type_e::CHAR:
            return "CHAR";
        default:
            return "UNKNOWN";
        }
    }
};

#endif // __DATA_TYPE_HPP__