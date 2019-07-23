//
// Created by alex on 5/28/19.
//

#include <types/Image.h>
#include <types/Color.h>
#include <boost/python.hpp>
#include <cmath>
#include <algorithm>
#include <random>
#include <numeric>

#include <iostream>
#include <fstream>

template<typename T>
T clamp(T x, T a, T b)
{
    if(x < a) return a;
    if(x > b) return b;
    return x;
}

struct Coord
{
    int x, y;
};

Color bilinear_interpolation(std::array<Color, 4> corners, std::array<Coord, 4> coords, double x, double y)
{
    double r = 0.0, g = 0.0, b = 0.0;
    double sum = 0.0;

    for(int i = 0; i < 4; i++)
    {
        double d = sqrt(
                (coords[i].x - x) * (coords[i].x - x) +
                (coords[i].y - y) * (coords[i].y - y));

        r += corners[i].r * d;
        g += corners[i].g * d;
        b += corners[i].b * d;

        sum += d;
    }

    r /= sum;
    g /= sum;
    b /= sum;

    return Color(
            static_cast<uint8_t>(r),
            static_cast<uint8_t>(g),
            static_cast<uint8_t>(b));
}


Image separate_blue_channel(const Image& image)
{
    Image new_image(image.width(), image.height());

    for(int row = 0; row < static_cast<int>(image.height()); row++)
        for(int column = 0; column < static_cast<int>(image.width()); column++)
        {
            auto color = image.pixelAt(row, column);
            new_image.setPixel(row, column, Color(0, 0, color.b));
        }

    return new_image;
}

Image separate_red_channel(const Image& image)
{
    Image new_image(image.width(), image.height());

    for(int row = 0; row < static_cast<int>(image.height()); row++)
        for(int column = 0; column < static_cast<int>(image.width()); column++)
        {
            auto color = image.pixelAt(row, column);
            new_image.setPixel(row, column, Color(color.r, 0, 0));
        }

    return new_image;
}

Image separate_green_channel(const Image& image)
{
    Image new_image(image.width(), image.height());

    for(int row = 0; row < static_cast<int>(image.height()); row++)
        for(int column = 0; column < static_cast<int>(image.width()); column++)
        {
            auto color = image.pixelAt(row, column);
            new_image.setPixel(row, column, Color(0, color.g, 0));
        }

    return new_image;
}

Image luminance_map(const Image& image)
{
    Image new_image(image.width(), image.height());

    for(int row = 0; row < static_cast<int>(image.height()); row++)
        for(int column = 0; column < static_cast<int>(image.width()); column++)
        {
            auto color = image.pixelAt(row, column);
            auto luminance = static_cast<long long int>(color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722);
            new_image.setPixel(row, column, Color(luminance, luminance, luminance));
        }

    return new_image;
}

Image pixel_sort(const Image& source, const Image& mask)
{
    struct Interval {int column, row_start, row_end; };

    std::vector<Interval> intervals;
    for(int column = 0; column < static_cast<int>(source.width()); column++)
    {
        int row = 0;
        while(row < static_cast<int>(source.height()))
        {
            int row_begin = row;
            while(row < static_cast<int>(source.height()) && mask.pixelAt(row, column).r == 255u)
                row ++;
            intervals.push_back({column, row_begin, row - 1});

            while(row < static_cast<int>(source.height()) && mask.pixelAt(row, column).r != 255u)
                row ++;
        }
    }

    Image result = source;

    std::vector<Color> pixels;
    for(const auto& [column, row_start, row_end]: intervals)
    {
        for(int row = row_start; row <= row_end; row++)
            pixels.push_back(result.pixelAt(row, column));

        std::sort(pixels.begin(), pixels.end(), [](const auto& lhs, const auto& rhs)
        {
            auto a = lhs.r * 0.2126 + lhs.g * 0.7152 + lhs.b * 0.0722;
            auto b = rhs.r * 0.2126 + rhs.g * 0.7152 + rhs.b * 0.0722;

            return a < b;
        });

        for(int row = row_start, i = 0; row <= row_end; row++, i++)
            result.setPixel(row, column, pixels[i]);

        pixels.clear();
    }

    return result;
}

Image pixel_distort_displace(const Image& source, const Image& map, double row_factor, double column_factor)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            double displacement = row_factor * (map.pixelAt(row, column).r / 255. * 2. - 1.);
            long long int column_displaced = static_cast<long long int>(displacement * column_factor) + column;
            long long int row_displaced= static_cast<long long int>(displacement * row_factor) + row;

            if((row_displaced >= 0 && row_displaced < static_cast<int>(source.height())) &&
               (column_displaced >= 0 && column_displaced < static_cast<int>(source.width()))
                    )
            {
                new_image.setPixel(row_displaced, column_displaced, source.pixelAt(row, column));
            }
        }

    for(int row = 0; row < static_cast<int>(source.height()); row++)
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            if(new_image.pixelAt(row, column).r == 0 &&
               new_image.pixelAt(row, column).g == 0 &&
               new_image.pixelAt(row, column).b == 0)
                new_image.setPixel(row, column, source.pixelAt(row, column));
        }

    return new_image;
}

Image displacement(const Image& source, const Image& map, double row_factor, double column_factor)
{
    Image new_image(source.width(), source.height());
    Image known_pixels(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            double displacement = (map.pixelAt(row, column).r / 255. * 2. - 1.);
            long long int column_displaced = static_cast<long long int>(displacement * column_factor) + column;
            long long int row_displaced= static_cast<long long int>(displacement * row_factor) + row;

            row_displaced = (row_displaced + static_cast<int>(source.height())) % static_cast<int>(source.height());
            column_displaced = (column_displaced + static_cast<int>(source.width())) % static_cast<int>(source.width());

            new_image.setPixel(row_displaced, column_displaced, source.pixelAt(row, column));
            known_pixels.setPixel(row_displaced, column_displaced, Color(1, 0, 0));
        }
    }

    int row_radius = std::max(1, static_cast<int>(row_factor)) * 3;
    int column_radius = std::max(1, static_cast<int>(column_factor)) * 3;

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            if(!known_pixels.pixelAt(row, column).r)
            {
                std::array<Color, 4> samples{};
                std::array<Coord, 4> coords{};
                char rowDir[] = {1, -1, 0, 0};
                char colDir[] = {0, 0, -1, 1};

                for(int i = 0; i < 4; i++)
                {
                    int row_ = row, column_ = column;
                    bool finished = false;

                    for(int l = 0; l < row_radius && !finished; l++)
                    {
                        row_ += rowDir[i];
                        column_ = column;
                        for(int k = 0; k < column_radius && !finished; k++)
                        {
                            column_ += colDir[i];

                            if(known_pixels.pixelAt(
                                    (row_ + static_cast<int>(source.height())) % static_cast<int>(source.height()),
                                    (column_ + static_cast<int>(source.width())) % static_cast<int>(source.width())).r)
                            {
                                coords[i] = {
                                        (column_ + static_cast<int>(source.width())) % static_cast<int>(source.width()),
                                        (row_ + static_cast<int>(source.height())) % static_cast<int>(source.height())};
                                samples[i] = new_image.pixelAt(
                                        (row_ + static_cast<int>(source.height())) % static_cast<int>(source.height()),
                                        (column_ + static_cast<int>(source.width())) % static_cast<int>(source.width()));
                                finished = true;
                            }
                        }
                    }
                }

                new_image.setPixel(row, column,
                        bilinear_interpolation(samples, coords, column, row));
                known_pixels.setPixel(row, column, Color(1, 0, 0));
            }
        }
    }

    return new_image;
}

Image lift_gain(const Image& source, double lift, double gain)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            auto color = source.pixelAt(static_cast<int>(row), static_cast<int>(column));
            auto r = color.r * (gain - lift) + lift;
            auto g = color.g * (gain - lift) + lift;
            auto b = color.b * (gain - lift) + lift;

            r = clamp(r, 0., 255.);
            g = clamp(g, 0., 255.);
            b = clamp(b, 0., 255.);

            new_image.setPixel(static_cast<int>(row), static_cast<int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                      static_cast<uint8_t>(b)));
        }

    return new_image;
}

Image gamma_(const Image& source, double gamma)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            auto color = source.pixelAt(static_cast<int>(row), static_cast<int>(column));
            auto r = pow(color.r, 1. / gamma);
            auto g = pow(color.g, 1. / gamma);
            auto b = pow(color.b, 1. / gamma);
            new_image.setPixel(static_cast<int>(row), static_cast<int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                      static_cast<uint8_t>(b)));
        }

    return new_image;
}

Image contrast(const Image& source, double contrast)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            auto color = source.pixelAt(static_cast<int>(row), static_cast<int>(column));
            auto r = color.r * (1. + contrast) * (-1.) - contrast / 2.;
            auto g = color.g * (1. + contrast) * (-1.) - contrast / 2.;
            auto b = color.b * (1. + contrast) * (-1.) - contrast / 2.;

            r = clamp(r, 0., 255.);
            g = clamp(g, 0., 255.);
            b = clamp(b, 0., 255.);

            new_image.setPixel(static_cast<int>(row), static_cast<int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                      static_cast<uint8_t>(b)));
        }

    return new_image;
}

Image brightness(const Image& source, double brightness)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
        for(int column = 0; column < static_cast<int>(source.width()); column++)
        {
            auto color = source.pixelAt(static_cast<int>(row), static_cast<int>(column));
            auto r = color.r + brightness * 255;
            auto g = color.g + brightness * 255;
            auto b = color.b + brightness * 255;

            r = clamp(r, 0., 255.);
            g = clamp(g, 0., 255.);
            b = clamp(b, 0., 255.);

            new_image.setPixel(static_cast<int>(row), static_cast<int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                      static_cast<uint8_t>(b)));
        }

    return new_image;
}

Image box_blur(const Image& source, double row_factor, double column_factor)
{
    static auto get_pixel_or_black = [](const Image& image, int row, int column)
    {
        if(row < static_cast<int>(image.height()) && column < static_cast<int>(image.width()) && row >= 0 && column >= 0)
            return image.pixelAt(row, column);
        return Color();
    };

    Image tmp(source.width(), source.height());
    {
        double weight = 1.0f / column_factor;
        int half = static_cast<int>(column_factor / 2);
        for(int row = 0; row < static_cast<int>(source.height()); row++)
            for(int column = 0; column < static_cast<int>(source.width()); column++)
            {
                double r = 0., g = 0., b = 0.;
                for(int i = -half; i <= half; i++)
                {
                    r += get_pixel_or_black(source, row, column + i).r * weight;
                    g += get_pixel_or_black(source, row, column + i).g * weight;
                    b += get_pixel_or_black(source, row, column + i).b * weight;
                }

                r = clamp(r, 0., 255.);
                g = clamp(g, 0., 255.);
                b = clamp(b, 0., 255.);

                tmp.setPixel(static_cast<int>(row), static_cast<int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                    static_cast<uint8_t>(b)));
            }
    }

    Image new_image(source.width(), source.height());

    {
        double weight = 1.0f / row_factor;
        int half = static_cast<int>(column_factor / 2);
        for(int row = 0; row < static_cast<int>(source.height()); row++)
            for(int column = 0; column < static_cast<int>(source.width()); column++)
            {
                double r = 0., g = 0., b = 0.;
                for(int i = -half; i <= half; i++)
                {
                    r += get_pixel_or_black(tmp, row + i, column).r * weight;
                    g += get_pixel_or_black(tmp, row + i, column).g * weight;
                    b += get_pixel_or_black(tmp, row + i, column).b * weight;
                }

                r = clamp(r, 0., 255.);
                g = clamp(g, 0., 255.);
                b = clamp(b, 0., 255.);

                new_image.setPixel(static_cast<int>(row), static_cast<int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                          static_cast<uint8_t>(b)));
            }
    }

    return new_image;
}

Image mask(const Image& source, const Image& mask)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(source.width()); column++)
            if(mask.pixelAt(row, column).r == 255u)
                new_image.setPixel(row, column, source.pixelAt(row, column));
            else new_image.setPixel(row, column, Color(0, 0, 0));
    }

    return new_image;
}

Image add(const Image& a, const Image& b)
{
    Image new_image(a.width(), a.height());

    for(int row = 0; row < static_cast<int>(a.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(a.width()); column++)
        {
            uint8_t r_ = a.pixelAt(row, column).r + b.pixelAt(row, column).r;
            uint8_t g_ = a.pixelAt(row, column).g + b.pixelAt(row, column).g;
            uint8_t b_ = a.pixelAt(row, column).b + b.pixelAt(row, column).b;

            new_image.setPixel(row, column, Color(r_, g_, b_));
        }
    }

    return new_image;
}

Image strict_add(const Image& a, const Image& b)
{
    Image new_image(a.width(), a.height());

    for(int row = 0; row < static_cast<int>(a.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(a.width()); column++)
        {
            int r_ = static_cast<int>(a.pixelAt(row, column).r) + static_cast<int>(b.pixelAt(row, column).r);
            int g_ = static_cast<int>(a.pixelAt(row, column).g) + static_cast<int>(b.pixelAt(row, column).g);
            int b_ = static_cast<int>(a.pixelAt(row, column).b) + static_cast<int>(b.pixelAt(row, column).b);

            r_ = clamp(r_, 0, 255);
            g_ = clamp(g_, 0, 255);
            b_ = clamp(b_, 0, 255);

            new_image.setPixel(row, column, Color(r_, g_, b_));
        }
    }

    return new_image;
}

Image multiply(const Image& a, const Image& b)
{
    Image new_image(a.width(), a.height());

    for(int row = 0; row < static_cast<int>(a.height()); row++)
    {
        for (int column = 0; column < static_cast<int>(a.width()); column++)
        {
            int r_ = static_cast<int>(a.pixelAt(row, column).r) * static_cast<int>(b.pixelAt(row, column).r);
            int g_ = static_cast<int>(a.pixelAt(row, column).g) * static_cast<int>(b.pixelAt(row, column).g);
            int b_ = static_cast<int>(a.pixelAt(row, column).b) * static_cast<int>(b.pixelAt(row, column).b);

            r_ /= 255;
            g_ /= 255;
            b_ /= 255;

            new_image.setPixel(row, column, Color(r_, g_, b_));
        }
    }

    return new_image;
}

namespace PerlinNoise
{
    double lerp(double a, double b, double x)
    {
        return a + x * (b - a);
    }

    double fade(double t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    int inc(int num)
    {
        return num + 1;
    }

    double grad(int hash, double x, double y, double z)
    {
        switch(hash & 0xF)
        {
            case 0x0: return  x + y;
            case 0x1: return -x + y;
            case 0x2: return  x - y;
            case 0x3: return -x - y;
            case 0x4: return  x + z;
            case 0x5: return -x + z;
            case 0x6: return  x - z;
            case 0x7: return -x - z;
            case 0x8: return  y + z;
            case 0x9: return -y + z;
            case 0xA: return  y - z;
            case 0xB: return -y - z;
            case 0xC: return  y + x;
            case 0xD: return -y + z;
            case 0xE: return  y - x;
            case 0xF: return -y - z;
            default: return 0;
        }
    }

    double perlin(const std::vector<int>& p, double x, double y, double z)
    {
        int xi = static_cast<int>(x) & 255;
        int yi = static_cast<int>(y) & 255;
        int zi = static_cast<int>(z) & 255;

        double xf = x - static_cast<int>(x);
        double yf = y - static_cast<int>(y);
        double zf = z - static_cast<int>(z);

        double u = fade(xf);
        double v = fade(yf);
        double w = fade(zf);

        int aaa, aba, aab, abb, baa, bba, bab, bbb;
        aaa = p[p[p[    xi ]+    yi ]+    zi ];
        aba = p[p[p[    xi ]+inc(yi)]+    zi ];
        aab = p[p[p[    xi ]+    yi ]+inc(zi)];
        abb = p[p[p[    xi ]+inc(yi)]+inc(zi)];
        baa = p[p[p[inc(xi)]+    yi ]+    zi ];
        bba = p[p[p[inc(xi)]+inc(yi)]+    zi ];
        bab = p[p[p[inc(xi)]+    yi ]+inc(zi)];
        bbb = p[p[p[inc(xi)]+inc(yi)]+inc(zi)];

        double x1, x2, y1, y2;
        x1 = lerp(grad (aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
        x2 = lerp(grad (aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
        y1 = lerp(x1, x2, v);
        x1 = lerp(grad (aab, xf, yf, zf - 1), grad (bab, xf - 1, yf , zf - 1), u);
        x2 = lerp(grad (abb, xf, yf - 1, zf - 1), grad (bbb, xf - 1, yf - 1, zf - 1), u);
        y2 = lerp(x1, x2, v);

        return (lerp(y1, y2, w) + 1) / 2;
    }

    Image perlin_noise(const Image& source, long long int seed, double z, long long int octaves, double persistence)
    {
        Image new_image(source.width(), source.height());

        std::mt19937 engine(seed);

        std::vector<int> permutation;
        permutation.resize(256);
        std::iota(std::begin(permutation), std::end(permutation), 0);
        std::shuffle(std::begin(permutation), std::end(permutation), engine);

        permutation.insert(permutation.end(), permutation.begin(), permutation.end());

        for(int row = 0; row < static_cast<int>(source.height()); row++)
        {
            for(int column = 0; column < static_cast<int>(source.width()); column++)
            {
                double frequency = 1;
                double amplitude = 1;
                double max_value = 0;
                double total = 0;

                const double x = static_cast<double>(row) / static_cast<int>(source.height());
                const double y = static_cast<double>(column) / static_cast<int>(source.width());

                for(int i = 0; i < octaves; i++)
                {
                    total += perlin(permutation, x, y, z);

                    max_value += amplitude;
                    amplitude *= persistence;
                    frequency *= 2;
                }

                auto a = static_cast<uint8_t>(total / max_value * 255);

                new_image.setPixel(row, column, Color(a, a, a));
            }
        }

        return new_image;
    }
}

Image discriminator_greater_than(const Image& source, long long int threshold)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(source.width()); column++)
            if(source.pixelAt(row, column).r >= threshold)
                new_image.setPixel(row, column, Color(255, 255, 255));
            else new_image.setPixel(row, column, Color(0, 0, 0));
    }

    return new_image;
}

Image discriminator_lesser_than(const Image& source, long long int threshold)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(source.width()); column++)
            if(source.pixelAt(row, column).r <= threshold)
                new_image.setPixel(row, column, Color(255, 255, 255));
            else new_image.setPixel(row, column, Color(0, 0, 0));
    }

    return new_image;
}

Image discriminator_range(const Image& source, long long int a, long long int b)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for(int column = 0; column < static_cast<int>(source.width()); column++)
            if(source.pixelAt(row, column).r >= a && source.pixelAt(row, column).r <= b)
                new_image.setPixel(row, column, Color(255, 255, 255));
            else new_image.setPixel(row, column, Color(0, 0, 0));
    }

    return new_image;
}


Image solid_color(const Image& source, Color color)
{
    Image new_image(source.width(), source.height());

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for (int column = 0; column < static_cast<int>(source.width()); column++)
            new_image.setPixel(row, column, color);
    }

    return new_image;
}

Image lens_distortion(const Image& source, double strength, double zoom)
{
    Image new_image(source.width(), source.height());

    int half_width = source.width() / 2;
    int half_height = source.height() / 2;

    strength = strength == 0 ? 0.0001 : strength;
    double correction_radius = sqrt(
            source.width() * source.width() +
            source.height() * source.height()) / strength;

    for(int row = 0; row < static_cast<int>(source.height()); row++)
    {
        for (int column = 0; column < static_cast<int>(source.width()); column++) {
            int new_row = row - half_height;
            int new_column = column - half_width;

            double distance = sqrt(new_row * new_row + new_column * new_column);
            double r = distance / correction_radius;
            double theta = r == 0 ? 1 : atan(r) / r;

            double source_row = half_height + theta * new_row * zoom;
            double source_column = half_width + theta * new_column * zoom;

            if (0 < source_row && source_row < source.height() &&
                0 < source_column && source_column < source.width())
            {
                double x = source_column - floor(source_column);
                double y = source_row - floor(source_row);

                new_image.setPixel(
                        row,
                        column,
                        bilinear_interpolation({
                            source.pixelAt(static_cast<unsigned int>(floor(source_row)), static_cast<unsigned int>(floor(source_column))), //0, 0
                            source.pixelAt(static_cast<unsigned int>(floor(source_row)), static_cast<unsigned int>(ceil(source_column))), //1, 0
                            source.pixelAt(static_cast<unsigned int>(ceil(source_row)), static_cast<unsigned int>(ceil(source_column))), //1, 1
                            source.pixelAt(static_cast<unsigned int>(ceil(source_row)), static_cast<unsigned int>(floor(source_column)))  //0, 1
                        },{
                            Coord{0, 0},
                            Coord{1, 0},
                            Coord{1, 1},
                            Coord{0, 1}
                        }, x, y));
            }
            else new_image.setPixel(row, column, Color(0, 0, 0));
        }
    }

    return new_image;
}

BOOST_PYTHON_MODULE(images_internal)
{
    using namespace boost::python;

    def("luminance_map", luminance_map);
    def("separate_blue_channel", separate_blue_channel);
    def("separate_red_channel", separate_red_channel);
    def("separate_green_channel", separate_green_channel);
    def("pixel_sort", pixel_sort);
    def("pixel_distort_displace", pixel_distort_displace);
    def("displacement", displacement);
    def("lift_gain", lift_gain);
    def("gamma", gamma_);
    def("contrast", contrast);
    def("brightness", brightness);
    def("box_blur", box_blur);
    def("mask", mask);
    def("strict_add", strict_add);
    def("add", add);
    def("multiply", multiply);
    def("perlin_noise", PerlinNoise::perlin_noise);
    def("discriminator_greater_than", discriminator_greater_than);
    def("discriminator_lesser_than", discriminator_lesser_than);
    def("discriminator_range", discriminator_range);
    def("solid_color", solid_color);
    def("lens_distortion", lens_distortion);
}