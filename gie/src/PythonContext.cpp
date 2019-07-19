//
// Created by alex on 12/29/18.
//

#include <gie/PythonContext.h>

#include <boost/python.hpp>
#include <gie/detail/PythonUtils.h>

static std::vector<ArgumentMetadata> fetchArguments(const PythonContext& context, boost::python::object callable)
{
    using namespace boost::python;

    auto inspect = context.inspect();
    auto signature = inspect.attr("signature")(callable);

    std::vector<object> parameters{
            stl_input_iterator<object>(signature.attr("parameters").attr("items")()),
            stl_input_iterator<object>{}
    };

    std::vector<ArgumentMetadata> arguments{};
    arguments.reserve(parameters.size());

    for(const auto& parameter: parameters)
    {
        auto p = parameter[1];
        arguments.push_back(ArgumentMetadata{
                extract<std::string>(p.attr("name")),
                Type{extract<std::string>(p.attr("annotation").attr("__name__"))}
        });
    }

    return arguments;
}

static Type fetchReturnType(const PythonContext& context, boost::python::object callable)
{
    using namespace boost::python;

    auto inspect = context.inspect();
    auto signature = inspect.attr("signature")(callable);

    if(hasattr(signature, "return_annotation"))
        return Type{extract<std::string>(signature.attr("return_annotation").attr("__name__"))};
    else return Type{""};
}

static SymbolName createSymbolName(std::string qualifiedName)
{
    return SymbolName{
            qualifiedName.substr(qualifiedName.find_last_of('.') + 1),
            qualifiedName.substr(0, qualifiedName.find_last_of('.')),
            std::move(qualifiedName)
    };
}

static Symbol createSymbol(const PythonContext& context, boost::python::object callable, std::string qualifiedFunctionName)
{
    return Symbol
            {
                    createSymbolName(qualifiedFunctionName),
                    fetchArguments(context, callable),
                    fetchReturnType(context, callable),
                    callable
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
                auto qualifiedName = name + '.' + extractor();
                auto id = getSymbolId(qualifiedName);
                if(!id)
                {
                    m_importedSymbols.push_back(createSymbol(*this, o, qualifiedName));
                    m_nameSymbolIdMap[qualifiedName] = SymbolId{m_importedSymbols.size() - 1};
                }
                else
                    m_importedSymbols[id->get()] = createSymbol(*this, o, qualifiedName);
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

const Symbol* PythonContext::getSymbol(SymbolId id) const
{
    if(id.get() < m_importedSymbols.size())
        return &m_importedSymbols[id.get()];
    return nullptr;
}

const Symbol* PythonContext::getSymbol(const std::string& name) const
{
    auto id = getSymbolId(name);
    if(id)
        return getSymbol(*id);
    return nullptr;
}

std::optional<SymbolId> PythonContext::getSymbolId(const std::string& name) const
{
    if(auto it = m_nameSymbolIdMap.find(name); it != m_nameSymbolIdMap.end())
        return it->second;
    return std::nullopt;
}