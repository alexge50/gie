//
// Created by alex on 12/3/18.
//

#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Program.h>
#include <gie/ScriptGraph/Execute.h>

#include <boost/python.hpp>

#include <utility>

#include <iostream>

std::optional<Value> Program::run()
{
    auto result = executeGraph(m_pythonContext, m_graph);
    if(!result.empty())
        return result[0];
    return std::nullopt;
}

NodeId Program::addNode(const Node &node)
{
    return ::addNode(m_graph, node);
}

void Program::editNode(NodeId id, const Node &node)
{
    return ::editNode(m_graph, id, node);
}

void Program::removeNode(NodeId id)
{
    return ::removeNode(m_graph, id);
}

void Program::import(const std::string &module)
{
    m_pythonContext.importModule(module);
}