//
// Created by alex on 5/23/19.
//

#include <boost/python.hpp>
#include <Color.h>
#include <Image.h>

using namespace boost::python;

template<typename T>
const T copyObject(const T& v)
{
    return v;
}

BOOST_PYTHON_MODULE(internals)
{
    class_<Color>("Color", init<double, double, double>())
            .def("__copy__", copyObject<Color>)
            .def_readwrite("r", &Color::r)
            .def_readwrite("g", &Color::g)
            .def_readwrite("b", &Color::b);

    class_<Image>("Image", init<unsigned, unsigned >())
            .def("__copy__", copyObject<Image>)
            .def("pixel_at", &Image::pixelAt)
            .def("set_pixel", &Image::setPixel)
            .def("width", &Image::width_)
            .def("height", &Image::height_);
}