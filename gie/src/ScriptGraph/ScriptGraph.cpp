//
// Created by alex on 2/16/19.
//

#include <gie/ScriptGraph/ScriptGraph.h>

#include <optional>
#include <algorithm>
#include <functional>
#include <type_traits>

static const unsigned long long NotFound = -1;

static unsigned long long lookup(const ScriptGraph& graph, NodeId id)
{
    auto it = std::lower_bound(graph.nodes.begin(), graph.nodes.end(), id, [](auto node, auto id)
    {
        return node.second.get() < id.get();
    });

    return it == graph.nodes.end() ? NotFound : std::distance(graph.nodes.begin(), it);
}

Expected<NodeCachePair, NodeInterfaceError> getNode(ScriptGraph& graph, NodeId id)
{
    const auto r = lookup(graph, id);

    if(r == NotFound)
        return Expected<NodeCachePair, NodeInterfaceError>{makeUnexpected(NodeInterfaceError{NodeInterfaceError::errors::IncorrectNodeId})};

    return Expected<NodeCachePair, NodeInterfaceError>{{&graph.nodes[r].first, &graph.cache[r].first}};
}

Expected<ConstNodeCachePair, NodeInterfaceError> getNode(const ScriptGraph& graph, NodeId id)
{
    const auto r = lookup(graph, id);

    if(r == NotFound)
        return Expected<ConstNodeCachePair, NodeInterfaceError>{makeUnexpected(NodeInterfaceError{NodeInterfaceError::errors::IncorrectNodeId})};


    return Expected<ConstNodeCachePair, NodeInterfaceError>{{&graph.nodes[r].first, &graph.cache[r].first}};
}

NodeId addNode(ScriptGraph& graph, const Node& node)
{
    NodeId id{graph.nodes.empty() ? 0 : graph.nodes.back().second.get() + 1};

    graph.nodes.emplace_back(node, id);
    graph.cache.emplace_back(std::nullopt, id);

    graph.structure.addNode(id);

    updateNode(graph, id).discard();

    return id;
}

MaybeError<NodeInterfaceError> editNode(ScriptGraph& graph, NodeId id, ArgumentId argumentId, ArgumentValue value)
{
    auto r = lookup(graph, id);

    if(r != NotFound)
    {
        auto& argument = graph.nodes[r].first.arguments[argumentId.get()];
        if(std::holds_alternative<NodeId>(argument))
        {
            auto otherId = std::get<NodeId>(argument);
            graph.structure.removeEdge(otherId, id);
        }

        argument = std::move(value);
        if(std::holds_alternative<NodeId>(argument))
        {
            auto otherId = std::get<NodeId>(argument);
            graph.structure.addEdge(otherId, id);
        }

        return {};
    }

    return {NodeInterfaceError{NodeInterfaceError::errors::IncorrectNodeId}};
}

MaybeError<NodeInterfaceError> updateNode(ScriptGraph& graph, NodeId id)
{
    if(lookup(graph, id) == NotFound)
        return {NodeInterfaceError{NodeInterfaceError::errors::IncorrectNodeId}};

    std::vector<NodeId> toRemove;
    toRemove.reserve(graph.structure.inDegree(id));

    graph.structure.iterateInNeighbours(id, [&toRemove](NodeId id)
    {
        toRemove.push_back(id);
    });

    for(auto otherId: toRemove)
        graph.structure.removeEdge(id, otherId);

    for(const auto &argument: getNode(graph, id)->node->arguments)
    {
        if(std::holds_alternative<NodeId>(argument))
        {
            auto otherId = std::get<NodeId>(argument);
            graph.structure.addEdge(otherId, id);
        }
    }

    return {};
}

MaybeError<NodeInterfaceError> removeNode([[maybe_unused]]ScriptGraph& graph, [[maybe_unused]]NodeId id)
{
    auto r = lookup(graph, id);

    if(r == NotFound)
        return {NodeInterfaceError{NodeInterfaceError::errors::IncorrectNodeId}};

    graph.nodes.erase(graph.nodes.begin() + r);
    graph.cache.erase(graph.cache.begin() + r);

    graph.structure.removeNode(id);

    return {};
}

void addResult(ScriptGraph& graph, std::string tag, NodeId id)
{
    graph.results.emplace_back(tag, id);
}
void editResult(ScriptGraph& graph, std::string tag, NodeId id)
{
    auto it = std::find_if(graph.results.begin(), graph.results.end(), [&tag](auto x) { return x.first == tag; });
    if(it != graph.results.end())
        it->second = id;
}

void removeResult(ScriptGraph& graph, std::string tag)
{
    graph.results.erase(
            std::remove_if(graph.results.begin(), graph.results.end(), [&tag](auto x) { return x.first == tag; }),
            graph.results.end()
    );
}