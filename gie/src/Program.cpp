//
// Created by alex on 12/3/18.
//

#include <gie/Program.h>

#include <boost/python.hpp>

#include <gie/execute/Execute.h>

std::optional<Value> Program::run()
{
    return {};
}

NodeId Program::addNode(const Node &node)
{
    return m_graph.addNode(node);
}

void Program::editNode(NodeId id, const Node &node)
{
}

void Program::removeNode(NodeId id)
{
    m_graph.removeNode(id);
}
