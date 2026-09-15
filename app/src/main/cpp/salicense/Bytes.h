#ifndef SALICENSE_BYTES_H
#define SALICENSE_BYTES_H

#include <vector>
#include <string>
#include <cstdint>

namespace SALicense
{
    using ByteArray = std::vector<uint8_t>;

    class Bytes
    {
    public:
        static std::string toHex(const ByteArray& data);

        static std::string toAscii(const ByteArray& data);

        static ByteArray slice(
            const ByteArray& data,
            size_t offset,
            size_t length
        );

        static uint32_t readUInt32(
            const ByteArray& data,
            size_t offset
        );
    };
}

#endif