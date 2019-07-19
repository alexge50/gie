//
// Created by alex on 5/23/19.
//

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <types/Color.h>
#include <types/Image.h>

#include <cstring>

namespace py = boost::python;
namespace np = boost::python::numpy;

template<typename T>
const T copyObject(const T& v)
{
    return v;
}

Image to_image(const np::ndarray& np_image)
{
    np::initialize();

    std::size_t width = np_image.shape(1);
    std::size_t height = np_image.shape(0);

    std::cout << width << " " << height << std::endl;

    Image image{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
    std::size_t offset = 0;
    for(unsigned int j = 0; j < image.height(); j++)
    {
        for (unsigned int i = 0; i < image.height(); i++)
        {
            Color pixel;

            pixel.r = static_cast<uint8_t>((*reinterpret_cast<float*>(np_image.get_data() + offset + sizeof(float) * 0)) * 255.f);
            pixel.g = static_cast<uint8_t>((*reinterpret_cast<float*>(np_image.get_data() + offset + sizeof(float) * 1)) * 255.f);
            pixel.b = static_cast<uint8_t>((*reinterpret_cast<float*>(np_image.get_data() + offset + sizeof(float) * 2)) * 255.f);

            image.setPixel(j, i, pixel);
            offset += sizeof(float) * 3;
        }
    }

    return image;
}

np::ndarray to_ndarray(const Image& image)
{
    np::initialize();

    py::tuple shape = py::make_tuple(image.height(), image.width(), 3);
    auto np_image = np::zeros(shape, np::dtype::get_builtin<float>());

    std::size_t offset = 0;
    for(unsigned int j = 0; j < image.height(); j++)
    {
        for(unsigned int i = 0; i < image.height(); i++)
        {
            Color pixel = image.pixelAt(j, i);

            *reinterpret_cast<float*>(np_image.get_data() + offset) = static_cast<float>(pixel.r) / 255.;
            *reinterpret_cast<float*>(np_image.get_data() + offset + sizeof(float)) = static_cast<float>(pixel.g) / 255.;
            *reinterpret_cast<float*>(np_image.get_data() + offset + sizeof(float) * 2) = static_cast<float>(pixel.b) / 255.;

            offset += sizeof(float) * 3;
        }
    }

    return np_image;
}

BOOST_PYTHON_MODULE(internals)
{
    py::class_<Color>("Color", py::init<double, double, double>())
            .def("__copy__", copyObject<Color>)
            .def_readwrite("r", &Color::r)
            .def_readwrite("g", &Color::g)
            .def_readwrite("b", &Color::b);

    py::class_<Image>("Image", py::init<unsigned, unsigned >())
            .def("__copy__", copyObject<Image>)
            .def("pixel_at", &Image::pixelAt)
            .def("set_pixel", &Image::setPixel)
            .def("width", &Image::width)
            .def("height", &Image::height);

    py::def("to_ndarray", to_ndarray);
    py::def("to_image", to_image);
}