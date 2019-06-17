//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_VALUE_H
#define GIE_LIBRARY_VALUE_H

#include <gie/Type.h>

#include <string>
#include <boost/python.hpp>

struct Value
{
public:
    Value() {}
    explicit Value(boost::python::object object): m_object(std::move(object)) {}

    Type m_typeName; // TODO: unpopulated, typechecking is done somehow else, force typechecking in libgie.
    boost::python::object m_object;
};


#endif //GIE_LIBRARY_VALUE_H
