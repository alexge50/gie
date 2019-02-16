//
// Created by alex on 12/29/18.
//

#include <gie/execute/Execute.h>

#include <gie/Value.h>
#include <gie/SceneGraph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <boost/python.hpp>

#include <algorithm>
#include <utility>

Value executeNode(const PythonContext &context, ScriptGraph &graph, NodeId nodeId)
{
    using namespace boost::python;

    auto node_ = getNode(graph, nodeId);
    list arguments;

    for(const auto &[name, argument]: node_.node.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument))
            arguments.append(object{getNode(graph, std::get<NodeId>(argument)).cache->m_object});
        else
            arguments.append(std::get<Value>(argument).m_object);
    }

    auto p = PyEval_CallObject(context.getFunction(node_.node.m_logic.m_functionName).ptr(), tuple{arguments}.ptr());
    object r{handle(borrowed(p))};

    node_.cache = {"123", r};

    return {"", r};
}

static void topologicalSort(
        const ScriptGraph::graph &structure,
        NodeId node,
        std::unordered_map<NodeId, bool> &visited,
        std::vector<std::pair<NodeId, bool>> &stack)
{
    bool unused = true;
    visited[node] = true;

    for(auto [it, end] = boost::vertices(structure); it != end; it++)
    {
        if(!visited[*it])
            topologicalSort(structure, *it, visited, stack);
        unused = unused && (node == *it);
    }

    stack.emplace_back(node, unused);
}

std::vector<std::pair<NodeId, bool>> calculateRuntimeOrder(const ScriptGraph::graph& structure)
{
    std::vector<std::pair<NodeId, bool>> stack;
    std::unordered_map<NodeId, bool> visited;

    for(auto [it, end] = boost::vertices(structure); it != end; it++)
    {
        if(!visited[*it])
            topologicalSort(structure, *it, visited, stack);
    }

    std::reverse(stack.begin(), stack.end());

    return stack;
}

std::vector<Value> executeGraph(const PythonContext &context, ScriptGraph &graph)
{
    std::vector<Value> result;
    auto runtimeOrder = calculateRuntimeOrder(graph.structure);

    for(auto [node, unused]: runtimeOrder)
    {
        auto r = executeNode(context, graph, node);
        getNode(graph, node).cache = r;
        if(unused)
            result.push_back(r);
    }

    return result;
}