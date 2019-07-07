//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_PYTHONCONTEXT_H
#define GIE_LIBRARY_PYTHONCONTEXT_H

#include <boost/python.hpp>
#include <unordered_map>
#include <string>

struct Symbol
{
    std::string prettyName;
    std::string module;
    std::string qualifiedName;
};

Symbol createSymbol(std::string qualifiedName);

class PythonContext
{
public:
    PythonContext();

    boost::python::object module(const std::string&, bool exposeSymbols = true);
    boost::python::object module(const std::string& name, const std::string& path, bool exposeSymbols = true);
    boost::python::object getFunction(const std::string&) const;

    const std::vector<Symbol>& importedSymbols() const { return m_importedSymbols; }

    boost::python::object inspect() const;

private:
    boost::python::object importAbsolute(const std::string& name, const std::string& path);
    void discoverSymbols(const std::string& name, boost::python::object);

private:
    boost::python::object m_main;
    boost::python::object m_global;
    boost::python::object m_importlib, m_importModule;
    boost::python::object m_inspect;

    std::unordered_map<std::string, boost::python::object> m_importedModules;
    std::vector<Symbol> m_importedSymbols;
    std::unordered_map<std::string, boost::python::object> m_functions;
};

#endif //GIE_LIBRARY_PYTHONCONTEXT_H
