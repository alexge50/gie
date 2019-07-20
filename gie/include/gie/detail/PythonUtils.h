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

inline std::string fetchPythonException()
{
    namespace py = boost::python;

    PyObject *pType = nullptr, *pValue = nullptr, *pTraceback = nullptr;
    PyErr_Fetch(&pType, &pValue, &pTraceback);
    std::string ret("Unfetchable Python error");

    if(pType)
    {
        py::handle<> type(pType);
        py::extract<std::string> typeString{py::str(type)};

        if(typeString.check())
            ret = typeString();
        else
            ret = "Unknown exception type";
    }
    if(pValue)
    {
        py::handle<> value(pValue);
        py::extract<std::string> returned{py::str(value)};

        if(returned.check())
            ret +=  ": " + returned();
        else
            ret += std::string(": Unparseable Python error: ");
    }

    if(pTraceback)
    {
        py::handle<> traceback(pTraceback);
        py::object tb(py::import("traceback"));
        py::object fmt_tb(tb.attr("format_tb"));
        py::object tb_list(fmt_tb(traceback));
        py::object tb_str(py::str("\n").join(tb_list));
        py::extract<std::string> returned(tb_str);

        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }
    return ret;
}

#endif //GIE_PYTHONUTILS_H
