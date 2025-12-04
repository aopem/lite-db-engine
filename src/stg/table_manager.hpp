#ifndef __TABLE_MANAGER_HPP__
#define __TABLE_MANAGER_HPP__

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace litedb
{
    class TableManager
    {
    public:
        TableManager(std::filesystem::path base_dir);
        ~TableManager() = default;

        std::vector<std::string> List(std::string_view database) const;
        void Create(std::string_view database, std::string_view table);
        void Delete(std::string_view database, std::string_view table);
        bool Exists(std::string_view database, std::string_view table) const;

    private:
        std::filesystem::path _base_dir;
    };
}

#endif // __TABLE_MANAGER_HPP__
