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

static boost::python::object copy(const PythonContext& context, boost::python::object o)
{
    using namespace boost::python;
    return context.copy().attr("deepcopy")(o);
}

std::optional<Value> executeNode(const PythonContext& context, const Node& node)
{
    using namespace boost::python;

    list arguments;

    for(const auto &argument: node.arguments)
    {
        if(std::holds_alternative<Value>(argument))
            arguments.append(copy(context, std::get<Value>(argument).m_object));
        else return std::nullopt;
    }

    auto p = PyEval_CallObject(node.function().ptr(), tuple{arguments}.ptr());
    return Value{object{handle(borrowed(p))}};
}

void executeNode(const PythonContext& context, ScriptGraph &graph, NodeId nodeId)
{
    using namespace boost::python;

    auto node_ = getNode(graph, nodeId);
    list arguments;

    for(const auto &argument: node_->node->arguments)
    {
        if(std::holds_alternative<NodeId>(argument))
        {
            auto& cache = *getNode(graph, std::get<NodeId>(argument))->cache;

            if(!cache.has_value())
                executeNode(context, graph, std::get<NodeId>(argument));
            arguments.append(copy(context, cache->m_object));
        }
        else
            arguments.append(copy(context, std::get<Value>(argument).m_object));
    }

    auto p = PyEval_CallObject(node_->node->function().ptr(), tuple{arguments}.ptr());
    object r{handle(borrowed(p))};

    *(node_->cache) = Value{r};
}

std::vector<Result> executeGraph(const PythonContext& context, ScriptGraph &graph)
{
    for(auto& cache: graph.cache)
        cache.first = std::nullopt;

    for(const auto& node: graph.nodes)
        executeNode(context, graph, node.second);

    std::vector<Result> results;

    const auto& constGraph = graph;
    results.reserve(graph.results.size());
    for(const auto& p: constGraph.results)
        results.push_back({p.first, getNode(constGraph, p.second)->cache->value()});

    return results;
}