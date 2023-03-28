#ifndef __DATA_TYPE_HPP__
#define __DATA_TYPE_HPP__

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
        data_type_e type;
        size_t length;
    };
};

#endif // __DATA_TYPE_HPP__