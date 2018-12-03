//
// Created by alex on 12/3/18.
//

#include <gie/Program.h>

void Program::run()
{

}

std::optional<Value> Program::getResult() {
    return std::optional<Value>();
}

NodeId Program::addNode(const Node &node)
{
    return m_graph.addNode(node);
}

void Program::editNode(NodeId id, const Node &node)
{
    m_graph.getNode(id) = node;
}

void Program::removeNode(NodeId id)
{
    m_graph.removeNode(id);
}
