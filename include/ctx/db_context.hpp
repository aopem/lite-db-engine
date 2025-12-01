#ifndef __DB_CONTEXT_HPP__
#define __DB_CONTEXT_HPP__

#include <string>
#include <string_view>

namespace litedb::ctx
{
    class DbContext
    {
    public:
        DbContext() = default;
        ~DbContext() = default;

        void SetCurrentDatabase(std::string_view database);
        std::string_view GetCurrentDatabase();

    private:
        std::string _current_database = "";
    };
}

#endif // !__DB_CONTEXT_HPP__