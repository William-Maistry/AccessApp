#include "ReadBarcode.h"
#include "Barcode.h"
#include "BarcodeFormat.h"
#include "ImageView.h"
#include "ReaderOptions.h"
#include <vector>
#include <cstring>

using namespace ZXing;


static std::vector<uint8_t> CropImage(
        const uint8_t* data,
        int width,
        int height,
        int startY,
        int cropHeight);

static bool TryDecodeImage(
        const uint8_t* data,
        int width,
        int height,
        std::string& output)
{
    ReaderOptions options;

    options.setFormats(BarcodeFormat::PDF417);
    options.setTryHarder(true);
    options.setTryRotate(true);
    options.setTryInvert(true);

    ImageView image(
            data,
            width,
            height,
            ImageFormat::Lum
    );

    Barcode result = ReadBarcode(image, options);

    if (!result.isValid())
        return false;

    output = result.text();

    return true;
}

static bool TryTopCrop(
        const uint8_t* data,
        int width,
        int height,
        std::string& output)
{
    int cropHeight = height * 55 / 100;

    auto crop = CropImage(
            data,
            width,
            height,
            0,
            cropHeight
    );

    return TryDecodeImage(
            crop.data(),
            width,
            cropHeight,
            output
    );
}

static bool TryMiddleCrop(
        const uint8_t* data,
        int width,
        int height,
        std::string& output)
{
    int startY = height * 10 / 100;
    int cropHeight = height * 60 / 100;

    auto crop = CropImage(
            data,
            width,
            height,
            startY,
            cropHeight
    );

    return TryDecodeImage(
            crop.data(),
            width,
            cropHeight,
            output
    );
}

static bool TryBottomCrop(
        const uint8_t* data,
        int width,
        int height,
        std::string& output)
{
    int startY = height * 20 / 100;
    int cropHeight = height * 70 / 100;

    auto crop = CropImage(
            data,
            width,
            height,
            startY,
            cropHeight
    );

    return TryDecodeImage(
            crop.data(),
            width,
            cropHeight,
            output
    );
}

static std::vector<uint8_t> CropImage(
        const uint8_t* data,
        int width,
        int height,
        int startY,
        int cropHeight)
{
    std::vector<uint8_t> crop(width * cropHeight);

    for (int y = 0; y < cropHeight; y++)
    {
        memcpy(
                crop.data() + y * width,
                data + (startY + y) * width,
                width
        );
    }

    return crop;
}

std::vector<uint8_t> DecodePDF417Bytes(
        const uint8_t* data,
        int width,
        int height)
{
    if (data == nullptr)
        return {};

    if (width <= 0 || height <= 0)
        return {};

    ReaderOptions options;
    options.setFormats(BarcodeFormat::PDF417);
    options.setTryHarder(true);
    options.setTryRotate(true);
    options.setTryInvert(true);

    ImageView image(data, width, height, ImageFormat::Lum);
    Barcode result = ReadBarcode(image, options);
    if (result.isValid())
        return result.bytes();

    int cropHeight = height * 55 / 100;
    auto topCrop = CropImage(data, width, height, 0, cropHeight);
    image = ImageView(topCrop.data(), width, cropHeight, ImageFormat::Lum);
    result = ReadBarcode(image, options);
    if (result.isValid())
        return result.bytes();

    int middleStart = height * 10 / 100;
    int middleCropHeight = height * 60 / 100;
    auto middleCrop = CropImage(data, width, height, middleStart, middleCropHeight);
    image = ImageView(middleCrop.data(), width, middleCropHeight, ImageFormat::Lum);
    result = ReadBarcode(image, options);
    if (result.isValid())
        return result.bytes();

    int bottomStart = height * 20 / 100;
    int bottomCropHeight = height * 70 / 100;
    auto bottomCrop = CropImage(data, width, height, bottomStart, bottomCropHeight);
    image = ImageView(bottomCrop.data(), width, bottomCropHeight, ImageFormat::Lum);
    result = ReadBarcode(image, options);
    if (result.isValid())
        return result.bytes();

    return {};
}

std::string DecodePDF417(
        const uint8_t* data,
        int width,
        int height)
{
    if (data == nullptr)
        return "";

    if (width <= 0 || height <= 0)
        return "";

    std::string output;

    // Full frame
    if (TryDecodeImage(data, width, height, output))
        return output;

    output.clear();

    // Top crop
    if (TryTopCrop(data, width, height, output))
        return output;

    output.clear();

    // Middle crop
    if (TryMiddleCrop(data, width, height, output))
        return output;

    output.clear();

    // Bottom crop
    if (TryBottomCrop(data, width, height, output))
        return output;

    return "";
}