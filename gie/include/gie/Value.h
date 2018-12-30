//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_VALUE_H
#define GIE_LIBRARY_VALUE_H

#include <string>

#include <boost/python.hpp>

struct Value
{
    std::string m_typeName;
    boost::python::object m_object;
};


#endif //GIE_LIBRARY_VALUE_H
