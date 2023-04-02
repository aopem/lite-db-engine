#ifndef __DATA_TYPE_HPP__
#define __DATA_TYPE_HPP__

#include <string>

namespace engine::query
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

    struct data_entry_t
    {
        std::string value;
        data_type_t type;
    };
};

#endif // __DATA_TYPE_HPP__