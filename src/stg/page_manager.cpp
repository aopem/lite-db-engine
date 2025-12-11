#include "stg/page_manager.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <boost/log/trivial.hpp>

namespace litedb
{
    PageManager::PageManager(const std::filesystem::path &file_path)
        : _fd(-1),
          _page_count(0)
    {
        _fd = open(file_path.c_str(), O_RDWR | O_CREAT, 0644);
        if (_fd < 0)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to open file: " << file_path;
            return;
        }

        struct stat st;
        if (fstat(_fd, &st) == 0 && st.st_size > 0)
        {
            _page_count = static_cast<uint32_t>(st.st_size / PAGE_SIZE);
        }

        BOOST_LOG_TRIVIAL(debug) << "PageManager opened: " << file_path
                                 << ", pages: " << _page_count;
    }

    PageManager::~PageManager()
    {
        if (_fd >= 0)
        {
            fsync(_fd);
            close(_fd);
        }
    }

    uint32_t PageManager::AllocatePage()
    {
        auto page_id = _page_count;

        // write empty page to extend file
        auto new_page = Page{page_id};
        WritePage(new_page);

        _page_count++;
        BOOST_LOG_TRIVIAL(debug) << "Allocated page: " << page_id;
        return page_id;
    }

    std::optional<Page> PageManager::ReadPage(uint32_t page_id)
    {
        if (page_id >= _page_count)
        {
            BOOST_LOG_TRIVIAL(warning) << "Invalid page id: " << page_id;
            return std::nullopt;
        }

        auto buffer = std::array<uint8_t, PAGE_SIZE>{};
        auto offset = static_cast<off_t>(page_id) * PAGE_SIZE;

        auto bytes = pread(_fd, buffer.data(), PAGE_SIZE, offset);
        if (bytes != PAGE_SIZE)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to read page: " << page_id;
            return std::nullopt;
        }

        auto page = Page{};
        page.FromBytes(buffer.data());
        return page;
    }

    bool PageManager::WritePage(const Page &page)
    {
        auto buffer = std::array<uint8_t, PAGE_SIZE>{};
        page.ToBytes(buffer.data());

        auto offset = static_cast<off_t>(page.Header().page_id) * PAGE_SIZE;

        auto bytes = pwrite(_fd, buffer.data(), PAGE_SIZE, offset);
        if (bytes != PAGE_SIZE)
        {
            BOOST_LOG_TRIVIAL(error) << "Failed to write page: " << page.Header().page_id;
            return false;
        }

        return true;
    }

    void PageManager::Flush()
    {
        fsync(_fd);
    }

}
