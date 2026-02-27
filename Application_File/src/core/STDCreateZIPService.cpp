#include "STDCreateZIPService.h"

bool STDCreateZIPService::createZIP(const std::string& rawPath) {
    std::ofstream file(rawPath, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
        return false;

    // 22 bytes min correct empty zip
    static constexpr std::array<unsigned char, 22> emptyZip = {
        0x50, 0x4B, 0x05, 0x06, // EOCD signature
        0x00, 0x00,             // number of this disk
        0x00, 0x00,             // disk where central directory starts
        0x00, 0x00,             // number of central directory records on this disk
        0x00, 0x00,             // total number of central directory records
        0x00, 0x00, 0x00, 0x00, // size of central directory
        0x00, 0x00, 0x00, 0x00, // offset of start of central directory
        0x00, 0x00              // ZIP file comment length
    };

    file.write(reinterpret_cast<const char*>(emptyZip.data()), emptyZip.size());
    file.close();

    return file.good();
}