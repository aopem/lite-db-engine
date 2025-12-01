#include "ctx/db_context.hpp"

namespace litedb::ctx
{
    void DbContext::SetCurrentDatabase(std::string_view database)
    {
        _current_database = database;
    }

    std::string_view DbContext::GetCurrentDatabase()
    {
        return _current_database;
    }
}