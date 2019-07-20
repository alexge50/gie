//
// Created by alex on 12/3/18.
//

#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Program.h>
#include <gie/ScriptGraph/Execute.h>

#include <boost/python.hpp>

#include <utility>

#include <iostream>

MaybeError<std::vector<ExecutionInterfaceError>> Program::run()
{
    return ::executeGraph(m_pythonContext, m_graph);
}

NodeId Program::addNode(std::string name, Arguments arguments)
{
    return ::addNode(m_graph, makeNode(m_pythonContext, std::move(name), std::move(arguments)).value());
}

MaybeError<NodeInterfaceError> Program::editNode(NodeId id, ArgumentId argumentId, ArgumentValue argument)
{
    return ::editNode(m_graph, id, argumentId, std::move(argument));
}

MaybeError<NodeInterfaceError> Program::removeNode(NodeId id)
{
    return ::removeNode(m_graph, id);
}

Expected<const Node*, NodeInterfaceError> Program::getNode(NodeId id) const
{
    auto value = ::getNode(m_graph, id);

    if(!value)
        return Expected<const Node*, NodeInterfaceError>{makeUnexpected(value.error())};

    return Expected<const Node*, NodeInterfaceError>{value->node};
}

Expected<std::optional<Value>, NodeInterfaceError> Program::getCache(NodeId id) const
{
    auto value = ::getNode(m_graph, id);

    if(!value)
        return Expected<std::optional<Value>, NodeInterfaceError>{makeUnexpected(value.error())};

    return Expected<std::optional<Value>, NodeInterfaceError>{*value->cache};
}


void Program::addResult(std::string tag, NodeId id)
{
    ::addResult(m_graph, std::move(tag), id);
}

void Program::editResult(std::string tag, NodeId id)
{
    ::editResult(m_graph, std::move(tag), id);
}

void Program::removeResult(std::string tag)
{
    ::removeResult(m_graph, std::move(tag));
}

void Program::import(const std::string& name, const std::string& path)
{
    m_pythonContext.module(name, path);
}

