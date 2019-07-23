//
// Created by alex on 7/19/19.
//

#include <gie/Value.h>
#include <gie/detail/PythonUtils.h>

Value::Value(boost::python::object object):
    m_typeName{::type(object)},
    m_object(std::move(object))
{}

const Type& Value::type() const
{
    return m_typeName;
}

boost::python::object Value::object() const
{
    return m_object;
}