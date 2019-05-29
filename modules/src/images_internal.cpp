//
// Created by alex on 5/28/19.
//

#include <Image.h>
#include <Color.h>
#include <boost/python.hpp>
#include <cmath>

#include <iostream>
#include <fstream>

Image separate_blue_channel(const Image& image)
{
    Image new_image(image.width, image.height);

    for(int row = 0; row < image.height; row++)
        for(int column = 0; column < image.width; column++)
        {
            auto color = image.pixelAt(row, column);
            new_image.setPixel(row, column, Color(0, 0, color.b));
        }

    return new_image;
}

Image luminance_map(const Image& image)
{
    Image new_image(image.width, image.height);

    for(int row = 0; row < image.height; row++)
        for(int column = 0; column < image.width; column++)
        {
            auto color = image.pixelAt(row, column);
            auto luminance = static_cast<long long int>(color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722);
            new_image.setPixel(row, column, Color(luminance, luminance, luminance));
        }

    return new_image;
}

Image pixel_sort(const Image& source, const Image& mask, int threshold)
{
    struct Interval {int column, row_start, row_end; };

    std::vector<Interval> intervals;
    for(int column = 0; column < source.width; column++)
    {
        int row = 0;
        while(row < source.height)
        {
            int row_begin = row;
            while(row < source.height && mask.pixelAt(row, column).r >= threshold)
                row ++;
            intervals.push_back({column, row_begin, row - 1});

            while(row < source.height && mask.pixelAt(row, column).r < threshold)
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
            return std::make_tuple(lhs.r, lhs.g, lhs.b) < std::make_tuple(rhs.r, rhs.g, rhs.b);
        });

        for(int row = row_start, i = 0; row <= row_end; row++, i++)
            result.setPixel(row, column, pixels[i]);

        pixels.clear();
    }

    return result;
}

Image pixel_distort_displace(const Image& source, const Image& map, double row_factor, double column_factor)
{
    Image new_image(source.width, source.height);

    for(int row = 0; row < source.height; row++)
        for(int column = 0; column < source.width; column++)
        {
            double displacement = row_factor * (map.pixelAt(row, column).r / 255. * 2. - 1.);
            long long int column_displaced = static_cast<long long int>(displacement * column_factor) + column;
            long long int row_displaced= static_cast<long long int>(displacement * row_factor) + row;

            if((row_displaced >= 0 && row_displaced < source.height) &&
               (column_displaced >= 0 && column_displaced < source.width)
            )
            {
                new_image.setPixel(row_displaced, column_displaced, source.pixelAt(row, column));
            }
        }

    for(int row = 0; row < source.height; row++)
        for(int column = 0; column < source.width; column++)
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
    Image new_image(source.width, source.height);
    Image known_pixels(source.width, source.height);

    for(int row = 0; row < source.height; row++)
    {
        for(int column = 0; column < source.width; column++)
        {
            double displacement = (map.pixelAt(row, column).r / 255. * 2. - 1.);
            long long int column_displaced = static_cast<long long int>(displacement * column_factor) + column;
            long long int row_displaced= static_cast<long long int>(displacement * row_factor) + row;

            row_displaced = (row_displaced + source.height) % source.height;
            column_displaced = (column_displaced + source.width) % source.width;

            new_image.setPixel(row_displaced, column_displaced, source.pixelAt(row, column));
            known_pixels.setPixel(row_displaced, column_displaced, Color(1, 0, 0));
        }
    }

    std::ostringstream out;

    for(int row = 0; row < source.height; row++)
    {
        for(int column = 0; column < source.width; column++)
        {
            if(!known_pixels.pixelAt(row,column).r)
            {
                Color samples[4];
                double weights[4];
                int nSamples = 0;
                char rowDir[] = {1, -1, 0, 0};
                char colDir[] = {0, 0, -1, 1};

                out << "---\n";
                for(int i = 0; i < 4; i++)
                {
                    int row_ = row, column_ = column;
                    int dist = 0;

                    while(row_ >= 0 && row_ < source.height && column_ >= 0 && column_ < source.width && !known_pixels.pixelAt(row_,column_).r)
                    {
                        row_ += rowDir[i];
                        column_ += colDir[i];

                        dist ++;
                    }

                    if(row_ >= 0 && row_ < source.height && column_ >= 0 && column_ < source.width)
                    {
                        samples[i] = source.pixelAt(row_, column_);
                        weights[i] = 1. / dist;
                        nSamples++;
                    }
                    else weights[i] = 0;

                    out << "color found: " << row_ << " " << column_ << " " << dist << '(' << int(samples[i].r) << ',' << int(samples[i].g) << ',' << int(samples[i].b) << ")\n";
                }

                double r, g, b;

                if(nSamples != 0)
                {
                    r = (samples[0].r * weights[0] + samples[1].r * weights[1] + samples[2].r * weights[2] + samples[3].r * weights[3])/nSamples;
                    g = (samples[0].g * weights[0] + samples[1].g * weights[1] + samples[2].g * weights[2] + samples[3].g * weights[3])/nSamples;
                    b = (samples[0].b * weights[0] + samples[1].b * weights[1] + samples[2].b * weights[2] + samples[3].b * weights[3])/nSamples;
                }
                else r = g = b = 0;

                out << "computed color: " << '(' << int(r) << ',' << int(g) << ',' << int(b) << ")\n";

                new_image.setPixel(row, column, Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                      static_cast<uint8_t>(b)));
            }
        }
    }

    std::string s = out.str();

    std::ofstream("displacement_log") << s;

    return new_image;
}

Image lift_gain(const Image& source, double lift, double gain)
{
    Image new_image(source.width, source.height);

    for(int row = 0; row < source.height; row++)
        for(int column = 0; column < source.width; column++)
        {
            auto color = source.pixelAt(static_cast<unsigned int>(row), static_cast<unsigned int>(column));
            auto r = color.r * (gain - lift) + lift;
            auto g = color.g * (gain - lift) + lift;
            auto b = color.b * (gain - lift) + lift;
            new_image.setPixel(static_cast<unsigned int>(row), static_cast<unsigned int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                                        static_cast<uint8_t>(b)));
        }

    return new_image;
}

Image gamma_(const Image& source, double gamma)
{
    Image new_image(source.width, source.height);

    for(int row = 0; row < source.height; row++)
        for(int column = 0; column < source.width; column++)
        {
            auto color = source.pixelAt(static_cast<unsigned int>(row), static_cast<unsigned int>(column));
            auto r = pow(color.r, 1. / gamma);
            auto g = pow(color.g, 1. / gamma);
            auto b = pow(color.b, 1. / gamma);
            new_image.setPixel(static_cast<unsigned int>(row), static_cast<unsigned int>(column), Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                                                                                                        static_cast<uint8_t>(b)));
        }

    return new_image;
}

BOOST_PYTHON_MODULE(images_internal)
{
    using namespace boost::python;

    def("luminance_map", luminance_map);
    def("separate_blue_channel", separate_blue_channel);
    def("pixel_sort", pixel_sort);
    def("pixel_distort_displace", pixel_distort_displace);
    def("displacement", displacement);
    def("lift_gain", lift_gain);
    def("gamma", gamma_);
}