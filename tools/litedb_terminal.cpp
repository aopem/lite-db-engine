#include <litedb/lite_db_engine.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <unordered_map>

using namespace litedb;

void signal_handler(int)
{
    std::cout << "\nExiting..." << std::endl;
    std::exit(0);
}

const std::unordered_map<std::string_view, boost::log::trivial::severity_level> LOG_LEVELS = {
    {"--trace", boost::log::trivial::trace},
    {"--debug", boost::log::trivial::debug},
    {"--info", boost::log::trivial::info},
    {"--warning", boost::log::trivial::warning},
    {"--error", boost::log::trivial::error},
    {"--fatal", boost::log::trivial::fatal},
};

void usage(const char *program_name)
{
    std::cout << "Usage: " << program_name << " [options]\n"
              << "Options:\n"
              << "  --trace    Set logging level to trace\n"
              << "  --debug    Set logging level to debug\n"
              << "  --info     Set logging level to info (default)\n"
              << "  --warning  Set logging level to warning\n"
              << "  --error    Set logging level to error\n"
              << "  --fatal    Set logging level to fatal\n"
              << "  --help     Show this help message\n";
}

void set_logging_filter(boost::log::trivial::severity_level level)
{
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= level);
}

int main(int argc, char *argv[])
{
    boost::log::trivial::severity_level log_level = boost::log::trivial::info;

    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg = argv[i];

        if (arg == "--help")
        {
            usage(argv[0]);
            return 0;
        }

        if (auto it = LOG_LEVELS.find(arg); it != LOG_LEVELS.end())
        {
            log_level = it->second;
        }
        else
        {
            std::cerr << "Unknown option: " << arg << std::endl;
            usage(argv[0]);
            return 1;
        }
    }

    set_logging_filter(log_level);

    std::signal(SIGINT, signal_handler);

    LiteDbEngine engine;
    engine.RunInteractive();

    return 0;
}