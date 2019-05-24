//
// Created by alex on 5/24/19.
//

#ifndef MODULES_IMAGE_H
#define MODULES_IMAGE_H

#include <tuple>
#include <vector>
#include <cstdint>
#include <cstring>

#include <Color.h>

class Image
{
    static const int N_CHANNELS = 3;
public:
    Image(unsigned width, unsigned height):
        width{width},
        height{height},
        data(N_CHANNELS * width * height)
    {}

    Image(uint8_t* data, unsigned width, unsigned height):
            width{width},
            height{height},
            data(N_CHANNELS * width * height)
    {
        std::memcpy(this->data.data(), data, N_CHANNELS * width * height);
    }

    Color pixelAt(unsigned x, unsigned y)
    {
        uint8_t* pixel = &data[index(x, y)];
        return Color(pixel[0], pixel[1], pixel[2]);
    }

    void setPixel(unsigned x, unsigned y, Color color)
    {
        uint8_t* pixel = &data[index(x, y)];
        std::tie(pixel[0], pixel[1], pixel[2]) = color;
    }

private:
    std::size_t index(unsigned x, unsigned y)
    {
        return (y * width + x) * N_CHANNELS;
    }

private:
    std::vector<uint8_t> data;
    unsigned width, height;
};

#endif //MODULES_IMAGE_H
