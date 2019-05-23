//
// Created by alex on 5/18/19.
//

#include "mainwindow.h"
#include <QApplication>

#include <boost/python.hpp>
#include <iostream>

std::string fetchPythonException()
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

int main(int argc, char *argv[])
{
    int ret;
    try
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        ret = QApplication::exec();
    }
    catch(const boost::python::error_already_set&)
    {
        std::string errorString = fetchPythonException();
        std::cout << errorString << std::endl;
    }


    return ret;
}
