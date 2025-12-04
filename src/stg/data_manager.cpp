#include "stg/data_manager.hpp"

#include <fstream>
#include <sstream>

#include <boost/log/trivial.hpp>

namespace litedb
{
    DataManager::DataManager(std::filesystem::path base_dir)
        : _base_dir(std::move(base_dir)) {}

    void DataManager::Write(std::string_view database, std::string_view table, std::string_view data)
    {
        auto table_file = _base_dir / database / table;

        if (!std::filesystem::exists(table_file))
        {
            BOOST_LOG_TRIVIAL(warning) << "Table at path: " << table_file << " does not exist, cannot write data.";
            return;
        }

        auto file = fopen(table_file.c_str(), "a");
        if (file == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to open table at path: " << table_file << " for writing.";
            return;
        }

        fwrite(data.data(), sizeof(char), data.size(), file);
        fclose(file);
        BOOST_LOG_TRIVIAL(debug) << "Wrote data to table at path: " << table_file;
    }

    std::string DataManager::Read(std::string_view database, std::string_view table)
    {
        auto table_file = _base_dir / database / table;

        if (!std::filesystem::exists(table_file))
        {
            BOOST_LOG_TRIVIAL(warning) << "Table at path: " << table_file << " does not exist, cannot read data.";
            return "";
        }

        std::ifstream file(table_file);
        if (!file.is_open())
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to open table at path: " << table_file << " for reading.";
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        BOOST_LOG_TRIVIAL(debug) << "Read data from table at path: " << table_file;

        return buffer.str();
    }
}
