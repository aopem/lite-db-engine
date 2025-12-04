#ifndef __RESULT_HPP__
#define __RESULT_HPP__

#include <string>
#include <string_view>

namespace litedb
{
    template <typename T>
    struct Result
    {
        bool success = false;
        std::string error_message;
        T value;

        static Result<T> Ok(T val)
        {
            Result<T> result;
            result.success = true;
            result.value = std::move(val);
            return result;
        }

        static Result<T> Error(std::string_view message)
        {
            Result<T> result;
            result.success = false;
            result.error_message = std::string(message);
            return result;
        }

        bool HasValue() const { return success; }
        explicit operator bool() const { return success; }
    };

    // specialization for void (operations that don't return a value)
    template <>
    struct Result<void>
    {
        bool success = false;
        std::string error_message;

        static Result<void> Ok()
        {
            Result<void> result;
            result.success = true;
            return result;
        }

        static Result<void> Error(std::string_view message)
        {
            Result<void> result;
            result.success = false;
            result.error_message = std::string(message);
            return result;
        }

        bool HasValue() const { return success; }
        explicit operator bool() const { return success; }
    };
}

#endif // __RESULT_HPP__
