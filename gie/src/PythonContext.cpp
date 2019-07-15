//
// Created by alex on 12/29/18.
//

#include <gie/PythonContext.h>

#include <boost/python.hpp>

Symbol createSymbol(std::string qualifiedName)
{
    return Symbol{
        qualifiedName.substr(qualifiedName.find_last_of('.') + 1),
        qualifiedName.substr(0, qualifiedName.find('.')),
        std::move(qualifiedName)
    };
}

PythonContext::PythonContext()
{
    using namespace boost::python;

    Py_Initialize();

    m_main = import("__main__");
    m_global = m_main.attr("__dict__");

    m_importlib = import("importlib");
    m_importModule = m_importlib.attr("import_module");

    m_inspect = import("inspect");
    m_copy = import("copy");
}

boost::python::object PythonContext::module(const std::string& name, bool exposeSymbols)
{
    if(auto it = m_importedModules.find(name); it != m_importedModules.end())
        return it->second;

    auto module = m_importModule(name);
    m_importedModules.insert({
        name,
        module
    });

    if(exposeSymbols)
        discoverSymbols(name, module);

    return module;
}

boost::python::object PythonContext::module(const std::string& name, const std::string& path, bool exposeSymbols)
{
    if(auto it = m_importedModules.find(name); it != m_importedModules.end())
        return it->second;

    auto module = importAbsolute(name, path);
    m_importedModules.insert({
                                     name,
                                     module
                             });

    if(exposeSymbols)
        discoverSymbols(name, module);

    return module;
}

boost::python::object PythonContext::getFunction(const std::string &name) const
{
    if(auto it = m_functions.find(name); it != m_functions.end())
        return it->second;

    return boost::python::object();
}

boost::python::object PythonContext::importAbsolute(const std::string& name, const std::string& path)
{
    auto importlibUtil = module("importlib.util", false);
    auto spec = importlibUtil.attr("spec_from_file_location")(name, path);

    auto module = importlibUtil.attr("module_from_spec")(spec);
    spec.attr("loader").attr("exec_module")(module);

    return module;
}

void PythonContext::discoverSymbols(const std::string& name, boost::python::object module)
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

boost::python::object PythonContext::inspect() const
{
    return m_inspect;
}

boost::python::object PythonContext::copy() const
{
    return m_copy;
}