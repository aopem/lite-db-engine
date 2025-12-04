#ifndef __DB_CONTEXT_HPP__
#define __DB_CONTEXT_HPP__

#include <string>
#include <string_view>

namespace litedb
{
    class DbContext
    {
    public:
        DbContext() = default;
        ~DbContext() = default;

        void SetCurrentDatabase(std::string_view database);
        std::string_view GetCurrentDatabase() const;

    private:
        std::string _current_database = "";
    };
}

#endif // __DB_CONTEXT_HPP__