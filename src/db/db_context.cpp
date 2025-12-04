#include "ctx/db_context.hpp"

#include <boost/log/trivial.hpp>

namespace litedb
{
    void DbContext::SetCurrentDatabase(std::string_view database)
    {
        BOOST_LOG_TRIVIAL(debug) << "Setting current database to: " << database;
        _current_database = database;
    }

    std::string_view DbContext::GetCurrentDatabase()
    {
        return _current_database;
    }
}