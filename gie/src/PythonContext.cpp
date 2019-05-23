//
// Created by alex on 12/29/18.
//

#include <gie/PythonContext.h>

#include <boost/python.hpp>

Symbol createSymbol(std::string qualifiedName)
{
    return Symbol{
        qualifiedName.substr(0, qualifiedName.find('.')),
        qualifiedName.substr(qualifiedName.find_last_of('.') + 1),
        std::move(qualifiedName)
    };
}

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
            {
                auto extractor = boost::python::extract<std::string>(o.attr("__name__"));
                if(extractor.check())
                {
                    auto qualifiedName = name.substr(name.find_last_of('.') + 1) + '.' + extractor();
                    m_importedSymbols.push_back(createSymbol(qualifiedName));
                    m_functions[qualifiedName] = o;
                }
            }
        }
    }

    return module;
}

boost::python::object PythonContext::getFunction(const std::string &name) const
{
    if(auto it = m_functions.find(name); it != m_functions.end())
        return it->second;

    return boost::python::object();
}