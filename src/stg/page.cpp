#include "stg/page.hpp"

#include <span>

namespace litedb
{
    Page::Page() : _header{0, 0, 0}, _data{} {}

    Page::Page(uint32_t page_id) : _header{page_id, 0, 0}, _data{} {}

    std::optional<uint32_t> Page::Append(std::span<const uint8_t> data)
    {
        auto total_size = sizeof(RecordHeader) + data.size();
        if (total_size > FreeSpace())
        {
            return std::nullopt;
        }

        auto offset = _header.data_offset;

        // write record header
        RecordHeader rec_header{static_cast<uint32_t>(data.size())};
        std::memcpy(_data.data() + offset, &rec_header, sizeof(RecordHeader));
        offset += sizeof(RecordHeader);

        // write record data
        std::memcpy(_data.data() + offset, data.data(), data.size());

        _header.data_offset += static_cast<uint32_t>(total_size);
        _header.record_count++;

        return _header.record_count - 1;
    }

    std::optional<std::span<const uint8_t>> Page::Read(uint32_t record_index) const
    {
        if (record_index >= _header.record_count)
        {
            return std::nullopt;
        }

        size_t offset = 0;
        RecordHeader rec_header;
        for (uint32_t i = 0; i < record_index; i++)
        {
            std::memcpy(&rec_header, _data.data() + offset, sizeof(RecordHeader));
            offset += sizeof(RecordHeader) + rec_header.length;
        }

        std::memcpy(&rec_header, _data.data() + offset, sizeof(RecordHeader));
        offset += sizeof(RecordHeader);

        return std::span{_data.data() + offset, rec_header.length};
    }

    void Page::Clear()
    {
        _header.record_count = 0;
        _header.data_offset = 0;
        _data.fill(0);
    }

    void Page::ToBytes(uint8_t *buffer) const
    {
        std::memcpy(buffer, &_header, sizeof(PageHeader));
        std::memcpy(buffer + sizeof(PageHeader), _data.data(), PAGE_DATA_SIZE);
    }

    void Page::FromBytes(const uint8_t *buffer)
    {
        std::memcpy(&_header, buffer, sizeof(PageHeader));
        std::memcpy(_data.data(), buffer + sizeof(PageHeader), PAGE_DATA_SIZE);
    }

}
