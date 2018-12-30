//
// Created by alex on 12/29/18.
//

#include <gie/execute/Execute.h>

#include <gie/Value.h>
#include <gie/Graph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <boost/python.hpp>

#include <algorithm>
#include <utility>

Value executeNode(const PythonContext &context, Graph &graph, NodeId nodeId)
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

    auto r = call<object>(context.getFunction(node.m_logic.m_functionName).ptr(), arguments);
    return {"", r};
}

static void topologicalSort(
        Graph &graph,
        NodeId node,
        std::unordered_map<NodeId, bool> &visited,
        std::vector<std::pair<NodeId, bool>> &stack)
{
    bool unused = true;
    visited[node] = true;

    for(auto callee: graph.getCallees(node))
    {
        if(!visited[callee])
            topologicalSort(graph, callee, visited, stack);
        unused = false;
    }

    stack.emplace_back(node, unused);
}

std::vector<std::pair<NodeId, bool>> calculateRuntimeOrder(Graph &graph)
{
    std::vector<std::pair<NodeId, bool>> stack;
    std::unordered_map<NodeId, bool> visited;

    auto nodes = graph.getNodes();

    for(auto node: nodes)
    {
        if(!visited[node])
            topologicalSort(graph, node, visited, stack);
    }

    std::reverse(stack.begin(), stack.end());

    return std::move(stack);
}

std::vector<Value> executeGraph(const PythonContext &context, Graph &graph)
{
    std::vector<Value> result;
    auto runtimeOrder = calculateRuntimeOrder(graph);

    for(auto [node, unused]: runtimeOrder)
    {
        auto r = executeNode(context, graph, node);
        std::get<1>(graph.getNode(node)) = r;
        if(unused)
            result.push_back(r);
    }

    return std::move(result);
}