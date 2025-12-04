#include <litedb/lite_db_engine.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

using namespace litedb;

void set_logging_filter()
{
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::debug);
}

int main()
{
    set_logging_filter();

    LiteDbEngine engine;
    engine.RunInteractive();

    return 0;
}