#ifndef __DB_MANAGER_HPP__
#define __DB_MANAGER_HPP__

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace litedb
{
    class DbManager
    {
    public:
        DbManager(std::filesystem::path base_dir);
        ~DbManager() = default;

        std::vector<std::string> List() const;
        void Create(std::string_view database);
        void Delete(std::string_view database);
        bool Exists(std::string_view database) const;

    private:
        std::filesystem::path _base_dir;
    };
}

#endif // __DB_MANAGER_HPP__
