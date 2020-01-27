//
// Created by alex on 7/15/19.
//

#ifndef GIE_GIE_H
#define GIE_GIE_H

#include <unordered_set>

#include <QtCore/QtCore>
#include <QUuid>

#include <gie/Program.h>
#include <StrongAlias.h>

#include "GieNodeId.h"
#include "src/nodeeditor/TypeData.h"

#include <iostream>

struct GieSymbol
{
    SymbolName symbol;
    std::vector<ArgumentMetadata> arguments;
    Type returnType;
};

struct GieRuntimeError
{
    std::optional<QUuid> nodeId;
    std::string errorMessage;
};

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(GieNodeId)
Q_DECLARE_METATYPE(ArgumentId)
Q_DECLARE_METATYPE(std::vector<GieSymbol>)
Q_DECLARE_METATYPE(Data)
Q_DECLARE_METATYPE(GieRuntimeError)

class Gie: public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void init()
    {
        Py_Initialize();

        auto builtins = m_program.context().module("builtins", false);

        auto sys = m_program.context().module("sys", false);
        auto os = m_program.context().module("os", false);

        sys.attr("path").attr("insert")(1, os.attr("getcwd")());

        auto internals = m_program.context().module("modules.internals", false);

        boost::python::setattr(builtins, "Color", internals.attr("Color"));
        boost::python::setattr(builtins, "Image", internals.attr("Image"));
        boost::python::setattr(builtins, "to_image", internals.attr("to_image"));
        boost::python::setattr(builtins, "to_ndarray", internals.attr("to_ndarray"));
    }

    void addModulesDirectory(const std::string& path)
    {
        auto sys = m_program.context().module("sys", false);

        sys.attr("path").attr("insert")(1, path);
    }

    void removeModulesDirectory(const std::string& path)
    {
        auto sys = m_program.context().module("sys", false);

        try
        {
            sys.attr("path").attr("remove")(path);
        }
        catch(const boost::python::error_already_set&)
        {

        }
    }

    void addNode(GieNodeId id, const std::string& name)
    {
        if(auto it = m_map.find(id); it == m_map.end())
        {
            m_map[id] = m_program.addNode(name, {});
        }
    }

    void removeNode(GieNodeId id)
    {
        if(auto it = m_map.find(id); it != m_map.end())
        {
            m_program.removeNode(it->second).discard();
            m_map.erase(it);
        }
    }

    void editNode(GieNodeId id, ArgumentId port, const Data& data)
    {
        if(auto it = m_map.find(id); it != m_map.end())
        {
            auto error = m_program.editNode(it->second, port, std::visit([](auto x)
                {
                    return ArgumentValue{Value{boost::python::object{x}}};
                }, data));

            if(!error.errorSet())
                run();
        }
    }

    void editNode(GieNodeId id, ArgumentId port, GieNodeId nodeId)
    {
        if(auto it = m_map.find(id); it != m_map.end())
        {
            auto error = m_program.editNode(it->second, port, m_map[nodeId]);

            if(!error.errorSet())
                run();
        }
    }

    void removeArgument(GieNodeId id, ArgumentId port)
    {
        if(auto it = m_map.find(id); it != m_map.end())
        {
            m_program.editNode(it->second, port, NoArgument{}).discard();
        }
    }

    void loadModule(const std::string& name, const std::string& path)
    {
        auto module = m_program.import(name, path);

        if(!module)
            Q_EMIT runtimeError({std::nullopt, "coudln't load module: " + name + "(" + path + ")\n"});

        std::vector<GieSymbol> symbols;
        symbols.reserve(m_program.context().importedSymbols().size());
        for(const auto& symbol: m_program.context().importedSymbols())
        {
            if(symbol.name.module == name)
            {
                symbols.push_back({
                                          symbol.name,
                                          symbol.arguments,
                                          symbol.returnType
                                  });
            }
        }

        Q_EMIT symbolsAdded(symbols);

        run();
    }

    void addResultNotify(QUuid nodeId, GieNodeId id)
    {
        m_toNotify[nodeId] = m_map[id];

        auto cache = m_program.getCache(m_map[id]).value();
        if(cache.has_value())
                Q_EMIT resultUpdated(nodeId, toData(*cache));
    }

    void removeResultNotify(QUuid nodeId)
    {
        m_toNotify.erase(nodeId);
    }

Q_SIGNALS:
    void symbolsAdded(std::vector<GieSymbol>);
    void resultUpdated(QUuid, Data);
    void finished();
    void runtimeError(GieRuntimeError);

private:
    void run()
    {
        auto errors = m_program.run();

        if(errors.errorSet())
        {
            for(const auto& error: errors.error())
                if(error.error() == ExecutionInterfaceError::errors::PythonInternalError)
                    Q_EMIT runtimeError({
                        std::find_if(m_map.begin(), m_map.end(), [&error](const auto& p)
                        {
                            return error.id() == p.second;
                        })->first.get(),
                        error.detail().value()
                    });
        }

        for(const auto& id: m_toNotify)
        {
            auto cache = m_program.getCache(id.second).value();
            if(cache.has_value())
                Q_EMIT resultUpdated(id.first, toData(*cache));
        }
    }

private:
    Data toData(const Value& value)
    {
        const std::string& type = value.type().name();

        if(type == "int")
            return Data{value.extract<long long>()};
        else if(type == "float")
            return Data{value.extract<double>()};
        else if(type == "str")
            return Data{value.extract<std::string>()};
        else if(type == "Color")
            return Data{value.extract<Color>()};
        else if(type == "Image")
            return Data{value.extract<Image>()};

        return {};
    }

private:
    Program m_program;
    std::map<GieNodeId, NodeId> m_map;
    std::map<QUuid, NodeId> m_toNotify;
};

#endif //GIE_GIE_H
