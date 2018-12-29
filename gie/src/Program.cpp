//
// Created by alex on 12/3/18.
//

#include <gie/Program.h>

#include <boost/python.hpp>


Program::Program()
{
    Py_Initialize();
}

void Program::run()
{
    using namespace boost::python;

    object main = import("__main__");
    object global = main.attr("__dict__");
    auto nodes = m_graph.getNodes();

    for(auto nodeId: nodes)
    {
        auto node = m_graph.getNode(nodeId);
        auto &logic = node.m_logic;

        std::string function = logic.m_functionName + "()";
        object result = eval(function.c_str(), global, global);

        //TODO: arguments + dictionary with previous results.
    }
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
