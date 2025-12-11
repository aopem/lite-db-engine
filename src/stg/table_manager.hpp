#ifndef __TABLE_MANAGER_HPP__
#define __TABLE_MANAGER_HPP__

#include "stg/page_manager.hpp"
#include "stg/page.hpp"

#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace litedb
{
    struct RecordId
    {
        uint32_t page_id;
        uint32_t record_index;
    };

    class TableManager
    {
    public:
        TableManager(std::filesystem::path base_dir);
        ~TableManager() = default;

        std::vector<std::string> List(std::string_view database) const;
        void Create(std::string_view database, std::string_view table);
        void Delete(std::string_view database, std::string_view table);
        bool Exists(std::string_view database, std::string_view table) const;

        std::optional<RecordId> Insert(std::string_view database, std::string_view table, std::span<const uint8_t> data);
        std::optional<std::vector<uint8_t>> Read(std::string_view database, std::string_view table, RecordId id);

    private:
        std::filesystem::path _base_dir;
        std::unordered_map<std::string, std::shared_ptr<PageManager>> _page_managers;

        std::filesystem::path GetTableDataPath(std::string_view database, std::string_view table) const;
        std::shared_ptr<PageManager> GetOrOpenPageManager(std::string_view database, std::string_view table);
    };
}

#endif // __TABLE_MANAGER_HPP__
