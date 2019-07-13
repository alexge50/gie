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
    return ::executeGraph(m_pythonContext, m_graph);
}

NodeId Program::addNode(std::string name, Arguments arguments)
{
    return ::addNode(m_graph, makeNode(m_typeManager, std::move(name), std::move(arguments)).value());
}

void Program::editNode(NodeId id, size_t argumentId, ArgumentValue argument)
{
    return ::editNode(m_graph, id, argumentId, std::move(argument));
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

