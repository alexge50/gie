//
// Created by alex on 12/29/18.
//

#include <gie/PythonContext.h>

#include <boost/python.hpp>


PythonContext::PythonContext()
{
    using namespace boost::python;

    Py_Initialize();

    main = import("__main__");
    global = main.attr("__dict__");
}

boost::python::object PythonContext::module(const std::string& name)
{
    if(auto it = importedModules.find(name); it != importedModules.end())
        return it->second;

    auto module = boost::python::import(name.c_str());
    importedModules.insert({
        name,
        module
    });

    return module;
}

boost::python::object PythonContext::getFunction(const std::string &name) const
{
    for([[maybe_unused]] auto& [_, module]: importedModules)
    {
        if(module.attr("__dict__").contains(name.c_str()))
            return module.attr("__dict__")[name.c_str()];
    }

    return boost::python::object();
}