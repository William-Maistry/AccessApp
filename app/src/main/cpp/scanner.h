#pragma once

#include <string>
#include <vector>
#include <cstdint>

std::string DecodePDF417(
    const uint8_t* data,
    int width,
    int height
);

std::vector<uint8_t> DecodePDF417Bytes(
    const uint8_t* data,
    int width,
    int height
);
