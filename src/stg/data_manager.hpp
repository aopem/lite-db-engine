#ifndef __DATA_MANAGER_HPP__
#define __DATA_MANAGER_HPP__

#include <filesystem>
#include <string>
#include <string_view>

namespace litedb
{
    class DataManager
    {
    public:
        DataManager(std::filesystem::path base_dir);
        ~DataManager() = default;

        void Write(std::string_view database, std::string_view table, std::string_view data);
        std::string Read(std::string_view database, std::string_view table);

    private:
        std::filesystem::path _base_dir;
    };
}

#endif // __DATA_MANAGER_HPP__
