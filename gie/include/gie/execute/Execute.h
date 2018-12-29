//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_EXECUTE_H
#define GIE_LIBRARY_EXECUTE_H

#include <gie/Value.h>
#include <gie/Graph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <boost/python.hpp>

std::optional<Value> executeNode(const PythonContext &context, Graph &graph, NodeId nodeId)
{
    using namespace boost::python;

    auto [node, result] = graph.getNode(nodeId);
    dict arguments;

    for(const auto &[name, argument]: node.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument))
            arguments[name.m_argumentName] = std::get<1>(graph.getNode(std::get<NodeId>(argument)));
        else
            arguments[name.m_argumentName] = std::get<Value>(argument).m_object;

    }

    result.m_object = call<object>(context.getFunction(node.m_logic.m_functionName).ptr(), arguments);
}


#endif //GIE_LIBRARY_EXECUTE_H
