//
// Created by alex on 5/23/19.
//

#include <boost/python.hpp>
#include <Color.h>

using namespace boost::python;

BOOST_PYTHON_MODULE(internals)
{
    class_<Color>("Color", init<double, double, double>())
            .def_readwrite("r", &Color::r)
            .def_readwrite("g", &Color::g)
            .def_readwrite("b", &Color::b);
}