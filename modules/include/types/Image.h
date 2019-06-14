//
// Created by alex on 5/24/19.
//

#ifndef MODULES_IMAGE_H
#define MODULES_IMAGE_H

#include <tuple>
#include <vector>
#include <cstdint>
#include <cstring>

#include "Color.h"

class Image
{
    static const int N_CHANNELS = 3;
public:
    Image(unsigned width, unsigned height):
            width{width},
            height{height},
            data{new uint8_t[N_CHANNELS * width * height]}
    {}

    Image(const uint8_t* data, unsigned width, unsigned height):
            width{width},
            height{height},
            data{new uint8_t[N_CHANNELS * width * height]}
    {
        std::memcpy(this->data, data, N_CHANNELS * width * height);
    }

    Image(const std::vector<uint8_t>& vecdata, unsigned width, unsigned height):
        width{width},
        height{height},
        data{new uint8_t[N_CHANNELS * width * height]}
    {
        std::memcpy(data, vecdata.data(), N_CHANNELS * width * height);
    }

    Image(const Image& other):
            width{other.width},
            height{other.height},
            data{new uint8_t[N_CHANNELS * width * height]}
    {
        std::memcpy(data, other.data, N_CHANNELS * width * height);
    }

    Image(Image&&) = default;

    ~Image()
    {
        delete[] data;
    }

    Color pixelAt(unsigned row, unsigned column) const
    {
        const uint8_t* pixel = &data[index(row, column)];
        return Color(pixel[0], pixel[1], pixel[2]);
    }

    void setPixel(unsigned row, unsigned column, Color color)
    {
        uint8_t* pixel = &data[index(row, column)];
        std::tie(pixel[0], pixel[1], pixel[2]) = std::make_tuple(color.r, color.g, color.b);
    }

    const unsigned width, height;

    auto width_() { return width; }
    auto height_() { return height; }

    const uint8_t* raw() const { return data; }

private:
    std::size_t index(unsigned row, unsigned column) const
    {
        return (row * width + column) * N_CHANNELS;
    }

private:
    uint8_t* data;
};

#endif //MODULES_IMAGE_H
