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

BOOST_PYTHON_MODULE(images_internal)
{
    using namespace boost::python;

    def("luminance_map", luminance_map);
    def("separate_blue_channel", separate_blue_channel);
}