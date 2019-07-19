//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_PYTHONCONTEXT_H
#define GIE_LIBRARY_PYTHONCONTEXT_H

#include <boost/python.hpp>
#include <unordered_map>
#include <string>
#include <optional>

#include <gie/Argument.h>
#include <gie/Type.h>

#include <StrongAlias.h>

struct SymbolName
{
    std::string prettyName;
    std::string module;
    std::string qualifiedName;
};

struct Symbol
{
    SymbolName name;
    std::vector<ArgumentMetadata> arguments;
    Type returnType;
    boost::python::object function;

};

using SymbolId = StrongAlias<std::size_t, struct SymbolIdTag>;

class PythonContext
{
public:
    PythonContext();

    boost::python::object module(const std::string&, bool exposeSymbols = true);
    boost::python::object module(const std::string& name, const std::string& path, bool exposeSymbols = true);

    const std::vector<Symbol>& importedSymbols() const { return m_importedSymbols; }

    boost::python::object inspect() const;
    boost::python::object copy() const;

    const Symbol* getSymbol(SymbolId) const;
    const Symbol* getSymbol(const std::string&) const;
    std::optional<SymbolId>  getSymbolId(const std::string& name) const;

private:
    boost::python::object importAbsolute(const std::string& name, const std::string& path);
    void discoverSymbols(const std::string& name, boost::python::object);

private:
    boost::python::object m_main;
    boost::python::object m_global;
    boost::python::object m_importlib, m_importModule;
    boost::python::object m_inspect;
    boost::python::object m_copy;

    std::unordered_map<std::string, boost::python::object> m_importedModules;
    std::vector<Symbol> m_importedSymbols;
    std::unordered_map<std::string, SymbolId> m_nameSymbolIdMap;
};

#endif //GIE_LIBRARY_PYTHONCONTEXT_H
