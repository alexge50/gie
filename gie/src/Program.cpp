//
// Created by alex on 12/3/18.
//

#include <gie/Program.h>
#include <gie/execute/Execute.h>

#include <boost/python.hpp>

#include <utility>

std::optional<Value> Program::run()
{
    return executeGraph(m_pythonContext, m_graph)[0];
}

NodeId Program::addNode(const Node &node)
{
    auto id = m_graph.addNode(node);

    for(auto &argument: node.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument.second))
            m_graph.addCallee(std::get<NodeId>(argument.second), id);
    }

    return id;
}

void Program::editNode(NodeId id, const Node &node)
{
    Node toEdit = std::get<0>(m_graph.getNode(id));
    std::unordered_map<NodeId, bool> isCallee;

    for(auto &argument: node.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument.second))
        {
            isCallee[std::get<NodeId>(argument.second)] = true;
            m_graph.addCallee(std::get<NodeId>(argument.second), id);
        }
    }

    for(auto &argument: toEdit.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument.second) && !isCallee[std::get<NodeId>(argument.second)])
            m_graph.removeCallee(std::get<NodeId>(argument.second), id);
    }

    toEdit = node;
}

void Program::removeNode(NodeId id)
{
    m_graph.removeNode(id);
}

void Program::import(const std::string &module)
{
    m_pythonContext.importModule(module);
}