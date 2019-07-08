//
// Created by alex on 12/29/18.
//

#include <gie/ScriptGraph/Execute.h>

#include <gie/Value.h>
#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <boost/python.hpp>
#include <boost/graph/topological_sort.hpp>

#include <algorithm>
#include <utility>
#include <queue>

std::optional<Value> executeNode(const Node& node)
{
    using namespace boost::python;

    list arguments;

    for(const auto &argument: node.arguments)
    {
        if(std::holds_alternative<Value>(argument))
            arguments.append(std::get<Value>(argument).m_object);
        else return std::nullopt;
    }

    auto p = PyEval_CallObject(node.function().ptr(), tuple{arguments}.ptr());
    return Value{object{handle(borrowed(p))}};
}

void executeNode(ScriptGraph &graph, NodeId nodeId)
{
    using namespace boost::python;

    auto node_ = getNode(graph, nodeId);
    list arguments;

    for(const auto &argument: node_.node.arguments)
    {
        if(std::holds_alternative<NodeId>(argument))
        {
            if(!getNode(graph, std::get<NodeId>(argument)).cache.has_value())
                executeNode(graph, std::get<NodeId>(argument));
            arguments.append(object{getNode(graph, std::get<NodeId>(argument)).cache->m_object});
        }
        else
            arguments.append(std::get<Value>(argument).m_object);
    }

    auto p = PyEval_CallObject(node_.node.function().ptr(), tuple{arguments}.ptr());
    object r{handle(borrowed(p))};

    node_.cache = Value{r};
}

std::vector<Result> executeGraph(ScriptGraph &graph)
{
    for(auto& cache: graph.cache)
        cache.first = std::nullopt;

    for(const auto& node: graph.nodes)
        executeNode(graph, node.second);

    std::vector<Result> results;

    const auto& constGraph = graph;
    results.reserve(graph.results.size());
    for(const auto& p: constGraph.results)
        results.push_back({p.first, getNode(constGraph, p.second).cache.value()});

    return results;
}