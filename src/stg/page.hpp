#ifndef __PAGE_HPP__
#define __PAGE_HPP__

#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <span>

#include <unistd.h>

namespace litedb
{
    struct PageHeader
    {
        uint32_t page_id;
        uint32_t record_count;
        uint32_t data_offset;
    };

    constexpr size_t PAGE_SIZE = 4096;
    constexpr size_t PAGE_DATA_SIZE = PAGE_SIZE - sizeof(PageHeader);

    struct RecordHeader
    {
        uint32_t length;
    };

    /**
     * Simple fixed-size page
     *
     * Layout:
     * +------------------+
     * | PageHeader (12B) |
     * +------------------+
     * | RecordHeader 4B  |
     * | Record 0 data    |
     * +------------------+
     * | RecordHeader 4B  |
     * | Record 1 data    |
     * +------------------+
     * |       ...        |
     * +------------------+
     * | Free space       |
     * | Up to 4096B      |
     * +------------------+
     */
    class Page
    {
    public:
        Page();
        Page(uint32_t page_id);

        // header access
        const PageHeader &Header() const { return _header; }

        // data area access
        const uint8_t *Data() const { return _data.data(); }

        // get statistics about data area
        size_t FreeSpace() const { return PAGE_DATA_SIZE - _header.data_offset; }
        off_t PageDataOffset() const { return static_cast<off_t>(_header.page_id) * PAGE_SIZE; }

        // read/write to/from data area
        std::optional<uint32_t> Append(std::span<const uint8_t> data);
        std::optional<std::span<const uint8_t>> Read(uint32_t record_index) const;

        void Clear();

        void ToBytes(uint8_t *buffer) const;
        void FromBytes(const uint8_t *buffer);

    private:
        PageHeader _header;
        std::array<uint8_t, PAGE_DATA_SIZE> _data;
    };
}

#endif // __PAGE_HPP__
