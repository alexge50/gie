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
    explicit Value(boost::python::object object);

    const Type& type() const;
    boost::python::object object() const;

    template <typename T>
    T extract() const
    {
        return boost::python::extract<T>(m_object);
    }

private:
    Type m_typeName;
    boost::python::object m_object;
};


#endif //GIE_LIBRARY_VALUE_H
