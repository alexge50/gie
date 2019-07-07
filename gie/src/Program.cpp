//
// Created by alex on 12/3/18.
//

#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Program.h>
#include <gie/ScriptGraph/Execute.h>

#include <boost/python.hpp>

#include <utility>

#include <iostream>

std::vector<Result> Program::run()
{
    return executeGraph(m_graph);
}

NodeId Program::addNode(const Node &node)
{
    return ::addNode(m_graph, node);
}

NodeId Program::addNode(std::string name, Arguments arguments)
{
    return ::addNode(m_graph, makeNode(m_typeManager, name, arguments).value());
}

void Program::editNode(NodeId id, const Node &node)
{
    return ::editNode(m_graph, id, node);
}

void Program::editNode(NodeId id, size_t argumentId, ArgumentValue argument) //TODO: add ::editNode overload
{
    auto node = ::getNode(m_graph, id).node;
    node.arguments[argumentId] = std::move(argument);

    return ::editNode(m_graph, id, node);
}

void Program::removeNode(NodeId id)
{
    return ::removeNode(m_graph, id);
}

const Node& Program::getNode(NodeId id) const
{
    return ::getNode(m_graph, id).node;
}

void Program::addResult(std::string tag, NodeId id)
{
    ::addResult(m_graph, tag, id);
}

void Program::editResult(std::string tag, NodeId id)
{
    ::editResult(m_graph, tag, id);
}

void Program::removeResult(std::string tag)
{
    ::removeResult(m_graph, tag);
}

void Program::import(const std::string& name, const std::string& path)
{
    m_pythonContext.module(name, path);
}

