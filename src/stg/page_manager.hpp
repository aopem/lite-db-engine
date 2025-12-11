#ifndef __PAGE_MANAGER_HPP__
#define __PAGE_MANAGER_HPP__

#include "stg/page.hpp"

#include <filesystem>
#include <memory>
#include <optional>

namespace litedb
{
    class PageManager
    {
    public:
        PageManager(const std::filesystem::path &file_path);
        ~PageManager();

        // disable copying
        PageManager(const PageManager &) = delete;
        PageManager &operator=(const PageManager &) = delete;

        // allocate a new page at end of file
        uint32_t AllocatePage();

        // read/write page from/to disk
        std::optional<Page> ReadPage(uint32_t page_id);
        bool WritePage(const Page &page);

        uint32_t PageCount() const { return _page_count; }
        bool IsOpen() const { return _fd >= 0; }
        void Flush();

    private:
        int _fd;
        uint32_t _page_count;
    };

} // namespace litedb

#endif // __PAGE_MANAGER_HPP__
