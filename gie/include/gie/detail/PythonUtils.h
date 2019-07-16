//
// Created by alex on 7/14/19.
//

#ifndef GIE_PYTHONUTILS_H
#define GIE_PYTHONUTILS_H

#include <gie/PythonContext.h>

inline boost::python::object copy(const PythonContext& context, const boost::python::object& o)
{
    using namespace boost::python;
    return context.copy().attr("copy")(o);
}

inline bool hasattr(const boost::python::object& object, const char* name)
{
    return PyObject_HasAttrString(object.ptr(), name) != 0;
}

inline std::string type(const boost::python::object& o)
{
    return {o.ptr()->ob_type->tp_name};
}

#endif //GIE_PYTHONUTILS_H
