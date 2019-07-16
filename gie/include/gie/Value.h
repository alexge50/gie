//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_VALUE_H
#define GIE_LIBRARY_VALUE_H

#include <gie/Type.h>

#include <string>
#include <boost/python.hpp>

#include <gie/detail/PythonUtils.h>

struct Value
{
public:
    Value() {}
    explicit Value(boost::python::object object): m_typeName{::type(object)}, m_object(std::move(object)) {}

    const Type& type() const
    {
        return m_typeName;
    }

    boost::python::object object() const
    {
        return m_object;
    }

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
