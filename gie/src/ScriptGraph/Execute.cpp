//
// Created by alex on 12/29/18.
//

#include <gie/ScriptGraph/Execute.h>

#include <gie/Value.h>
#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <gie/detail/PythonUtils.h>

#include <boost/python.hpp>
#include <boost/graph/topological_sort.hpp>

#include <algorithm>
#include <utility>
#include <queue>


Expected<Value, ExecutionInterfaceError> executeNode(const PythonContext& context, const Node& node)
{
    using namespace boost::python;

    list arguments;

    for(const auto &argument: node.arguments)
    {
        if(std::holds_alternative<Value>(argument))
            arguments.append(copy(context, std::get<Value>(argument).object()));
        else return Expected<Value, ExecutionInterfaceError>{makeUnexpected(ExecutionInterfaceError{ExecutionInterfaceError::errors::InvalidArguments})};
    }

    try
    {
        auto p = PyEval_CallObject(context.getSymbol(node.symbolId())->function.ptr(), tuple{arguments}.ptr());
        return Expected<Value, ExecutionInterfaceError>{Value{object{handle(borrowed(p))}}};
    }
    catch(...)
    {
        return Expected<Value, ExecutionInterfaceError>{makeUnexpected(ExecutionInterfaceError{ExecutionInterfaceError::errors::PythonInternalError})};
    }
}

MaybeError<ExecutionInterfaceError> executeNode(const PythonContext& context, ScriptGraph &graph, NodeId nodeId)
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
            {
                auto error = executeNode(context, graph, std::get<NodeId>(argument));
                if(error)
                    return error;
            }
            arguments.append(copy(context, cache->object()));
        }
        else if(std::holds_alternative<Value>(argument))
            arguments.append(copy(context, std::get<Value>(argument).object()));
        else return ExecutionInterfaceError{ExecutionInterfaceError::errors::InvalidArguments};
    }

    try
    {
        auto p = PyEval_CallObject(context.getSymbol(node_->node->symbolId())->function.ptr(), tuple{arguments}.ptr());
        object r{handle(borrowed(p))};

        *(node_->cache) = Value{r};

    }
    catch(...)
    {
        return ExecutionInterfaceError{ExecutionInterfaceError::errors::PythonInternalError};
    }

    return {};
}

Expected<std::vector<Result>, ExecutionInterfaceError> executeGraph(const PythonContext& context, ScriptGraph &graph)
{
    for(auto& cache: graph.cache)
        cache.first = std::nullopt;

    for(const auto& node: graph.nodes)
    {
        auto error = executeNode(context, graph, node.second);
        if(error)
            return Expected<std::vector<Result>, ExecutionInterfaceError>{makeUnexpected(error.error())};
    }

    std::vector<Result> results;

    const auto& constGraph = graph;
    results.reserve(graph.results.size());
    for(const auto& p: constGraph.results)
        results.push_back({p.first, getNode(constGraph, p.second)->cache->value()});

    return Expected<std::vector<Result>, ExecutionInterfaceError>{results};
}