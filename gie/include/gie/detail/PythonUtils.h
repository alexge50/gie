//
// Created by alex on 7/14/19.
//

#ifndef GIE_PYTHONUTILS_H
#define GIE_PYTHONUTILS_H

#include <gie/PythonContext.h>

inline boost::python::object copy(const PythonContext& context, const boost::python::object& o)
{
    using namespace boost::python;
    return context.copy().attr("deepcopy")(o);
}

inline bool hasattr(const boost::python::object& object, const char* name)
{
    return PyObject_HasAttrString(object.ptr(), name) != 0;
}

inline std::string type(const boost::python::object& object)
{
    using namespace boost::python;

    auto type = getattr(object, "__class__");
    return extract<std::string>(getattr(type, "__name__"));
}

#endif //GIE_PYTHONUTILS_H
