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
    Image():
        m_width{0},
        m_height{0},
        data{nullptr}
    {}

    Image(unsigned width, unsigned height):
            m_width{width},
            m_height{height},
            data{new uint8_t[N_CHANNELS * width * height]}
    {}

    Image(const uint8_t* data, unsigned width, unsigned height):
            m_width{width},
            m_height{height},
            data{new uint8_t[N_CHANNELS * width * height]}
    {
        std::memcpy(this->data, data, N_CHANNELS * width * height);
    }

    Image(const std::vector<uint8_t>& vecdata, unsigned width, unsigned height):
        m_width{width},
        m_height{height},
        data{new uint8_t[N_CHANNELS * width * height]}
    {
        std::memcpy(data, vecdata.data(), N_CHANNELS * width * height);
    }

    Image(const Image& other):
            m_width{other.m_width},
            m_height{other.m_height},
            data{new uint8_t[N_CHANNELS * m_width * m_height]}
    {
        std::memcpy(data, other.data, N_CHANNELS * m_width * m_height);
    }

    Image(Image&& other):
        m_width{0},
        m_height{0},
        data{nullptr}
    {
        std::swap(other.m_width, m_width);
        std::swap(other.m_height, m_height);
        std::swap(other.data, data);
    }

    ~Image()
    {
        delete[] data;
    }

    Image& operator=(const Image& other)
    {
        delete[] data;
        data = new uint8_t[N_CHANNELS * m_width * m_height];
        m_width = other.m_width;
        m_height = other.m_height;

        std::memcpy(data, other.data, N_CHANNELS * m_width * m_height);

        return *this;
    }

    Image& operator=(Image&& other)
    {
        delete[] data;

        data = nullptr;
        m_width = 0;
        m_height = 0;

        std::swap(other.m_width, m_width);
        std::swap(other.m_height, m_height);
        std::swap(other.data, data);

        return *this;
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

    auto width() const { return m_width; }
    auto height() const { return m_height; }

    const uint8_t* raw() const { return data; }

private:
    std::size_t index(unsigned row, unsigned column) const
    {
        return (row * m_width + column) * N_CHANNELS;
    }

private:
    unsigned m_width, m_height;

    uint8_t* data;
};

#endif //MODULES_IMAGE_H
