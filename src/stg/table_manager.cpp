#include "stg/table_manager.hpp"

#include <boost/log/trivial.hpp>

namespace litedb
{
    TableManager::TableManager(std::filesystem::path base_dir)
        : _base_dir(std::move(base_dir)) {}

    std::filesystem::path TableManager::GetTableDataPath(std::string_view database, std::string_view table) const
    {
        return _base_dir / database / (std::string{table} + ".data");
    }

    std::shared_ptr<PageManager> TableManager::GetOrOpenPageManager(std::string_view database, std::string_view table)
    {
        auto path = GetTableDataPath(database, table);
        auto key = path.string();

        auto it = _page_managers.find(key);
        if (it != _page_managers.end())
        {
            return it->second;
        }

        auto mgr = std::make_shared<PageManager>(path);
        if (!mgr->IsOpen())
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to open PageManager for: " << path;
            return nullptr;
        }

        _page_managers[key] = mgr;
        return mgr;
    }

    std::vector<std::string> TableManager::List(std::string_view database) const
    {
        std::vector<std::string> tables;
        auto db_dir = _base_dir / database;
        if (!std::filesystem::exists(db_dir) || !std::filesystem::is_directory(db_dir))
        {
            BOOST_LOG_TRIVIAL(warning) << "Database directory: " << db_dir << " does not exist.";
            return tables;
        }

        BOOST_LOG_TRIVIAL(debug) << "Listing tables in database directory: " << db_dir;
        for (auto &entry : std::filesystem::directory_iterator(db_dir))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            auto filename = entry.path().filename().string();
            if (filename.ends_with(".data"))
            {
                tables.push_back(filename.substr(0, filename.size() - 5));
                BOOST_LOG_TRIVIAL(debug) << "Found table: " << filename;
            }
        }

        return tables;
    }

    void TableManager::Create(std::string_view database, std::string_view table)
    {
        auto path = GetTableDataPath(database, table);
        if (std::filesystem::exists(path))
        {
            BOOST_LOG_TRIVIAL(debug) << "Table already exists: " << path;
            return;
        }

        auto mgr = std::make_shared<PageManager>(path);
        if (!mgr->IsOpen())
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to create table: " << path;
            return;
        }

        _page_managers[path.string()] = mgr;
        BOOST_LOG_TRIVIAL(debug) << "Created table: " << path;
    }

    void TableManager::Delete(std::string_view database, std::string_view table)
    {
        auto path = GetTableDataPath(database, table);
        _page_managers.erase(path.string());

        if (std::filesystem::exists(path))
        {
            std::filesystem::remove(path);
            BOOST_LOG_TRIVIAL(debug) << "Deleted table: " << path;
        }
    }

    bool TableManager::Exists(std::string_view database, std::string_view table) const
    {
        return std::filesystem::exists(GetTableDataPath(database, table));
    }

    std::optional<RecordId> TableManager::Insert(std::string_view database, std::string_view table,
                                                 std::span<const uint8_t> data)
    {
        auto mgr = GetOrOpenPageManager(database, table);
        if (!mgr)
        {
            return std::nullopt;
        }

        auto record_size = sizeof(RecordHeader) + data.size();

        // try existing pages first
        for (uint32_t i = 0; i < mgr->PageCount(); i++)
        {
            auto page = mgr->ReadPage(i);
            if (!page)
            {
                continue;
            }

            if (page->FreeSpace() >= record_size)
            {
                auto record_idx = page->Append(data);
                if (record_idx != std::nullopt)
                {
                    mgr->WritePage(*page);
                    return RecordId{i, *record_idx};
                }
            }
        }

        // no space in existing pages, allocate new one
        auto page_id = mgr->AllocatePage();
        auto page = mgr->ReadPage(page_id);
        if (!page)
        {
            return std::nullopt;
        }

        auto record_idx = page->Append(data);
        if (!record_idx)
        {
            return std::nullopt;
        }

        mgr->WritePage(*page);
        return RecordId{page_id, *record_idx};
    }

    std::optional<std::vector<uint8_t>> TableManager::Read(std::string_view database, std::string_view table, RecordId id)
    {
        auto mgr = GetOrOpenPageManager(database, table);
        if (!mgr)
        {
            return std::nullopt;
        }

        auto page = mgr->ReadPage(id.page_id);
        if (!page)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to read page " << id.page_id;
            return std::nullopt;
        }

        auto record = page->Read(id.record_index);
        if (!record)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to read record " << id.record_index
                                     << " from page " << id.page_id;
            return std::nullopt;
        }

        return std::vector<uint8_t>{record->begin(), record->end()};
    }
}
