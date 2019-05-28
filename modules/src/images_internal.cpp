//
// Created by alex on 5/28/19.
//

#include <Image.h>
#include <Color.h>
#include <boost/python.hpp>

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

Image displacement(const Image& source, const Image& map, double row_factor, double column_factor)
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

    return new_image;
}

BOOST_PYTHON_MODULE(images_internal)
{
    using namespace boost::python;

    def("luminance_map", luminance_map);
    def("separate_blue_channel", separate_blue_channel);
    def("pixel_sort", pixel_sort);
    def("displacement", displacement);
}