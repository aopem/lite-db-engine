#include "stg/page.hpp"
#include "stg/page_manager.hpp"

#include <boost/log/trivial.hpp>

#include <cassert>
#include <filesystem>

using namespace litedb;

int main()
{
    BOOST_LOG_TRIVIAL(info) << "=== TestPageStorage ===";

    auto test_path = std::filesystem::path{"./.litedb/test_page_storage.data"};

    // cleanup before test
    if (std::filesystem::exists(test_path))
    {
        std::filesystem::remove(test_path);
    }

    {
        // write data
        auto mgr = PageManager{test_path};
        assert(mgr.IsOpen());

        mgr.AllocatePage();
        auto page = mgr.ReadPage(0);
        assert(page.has_value());

        page->Append(std::vector<uint8_t>{'H', 'e', 'l', 'l', 'o'});
        page->Append(std::vector<uint8_t>{'W', 'o', 'r', 'l', 'd'});

        mgr.WritePage(*page);
        mgr.Flush();
    }

    {
        // read data back
        auto mgr = PageManager{test_path};
        auto page = mgr.ReadPage(0);
        assert(page.has_value());
        assert(page->Header().record_count == 2);

        auto r1 = page->Read(0);
        auto r2 = page->Read(1);
        assert(r1.has_value() && r2.has_value());
        assert((*r1)[0] == 'H' && (*r2)[0] == 'W');
    }

    // cleanup
    std::filesystem::remove(test_path);

    BOOST_LOG_TRIVIAL(info) << "TestPageStorage PASSED";

    return 0;
}
