//
// Created by alex on 12/29/18.
//

#include <gie/PythonContext.h>

#include <boost/python.hpp>

PythonContext::PythonContext()
{
    using namespace boost::python;

    Py_Initialize();

    m_main = import("__main__");
    m_global = m_main.attr("__dict__");
}

boost::python::object PythonContext::module(const std::string& name, bool exposeSymbols)
{
    if(auto it = m_importedModules.find(name); it != m_importedModules.end())
        return it->second;

    auto module = boost::python::import(name.c_str());
    m_importedModules.insert({
        name,
        module
    });

    if(exposeSymbols)
    {
        const auto& list = (boost::python::extract<boost::python::dict>(module.attr("__dict__"))()).items();
        for(int i = 0; i < boost::python::len(list); i++)
        {
            auto o = boost::python::extract<boost::python::object>(list[i][1])();
            if(PyCallable_Check(o.ptr()))
                m_importedSymbols.push_back(boost::python::extract<std::string>(o.attr("__name__")));
        }
    }

    return module;
}

boost::python::object PythonContext::getFunction(const std::string &name) const
{
    for([[maybe_unused]] auto& [_, module]: m_importedModules)
    {
        if(module.attr("__dict__").contains(name.c_str()))
            return module.attr("__dict__")[name.c_str()];
    }

    return boost::python::object();
}