//
// Created by alex on 12/29/18.
//

#include <gie/PythonContext.h>

#include <boost/python.hpp>


PythonContext::PythonContext()
{
    using namespace boost::python;

    Py_Initialize();

    object main = import("__main__");
    object global = main.attr("__dict__");
}

void PythonContext::importModule(const std::string &name)
{
    importedModules.push_back(boost::python::import(name.c_str()));
}

boost::python::object PythonContext::getFunction(const std::string &name) const
{
    for(auto &module: importedModules)
    {
        if(module.attr("__dict__").contains(name.c_str()))
            return module.attr("__dict__")[name.c_str()];
    }

    return boost::python::object();
}