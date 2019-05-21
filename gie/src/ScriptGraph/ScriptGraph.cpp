//
// Created by alex on 2/16/19.
//

#include <gie/ScriptGraph/ScriptGraph.h>

#include <optional>
#include <functional>
#include <type_traits>

static long lookup(const ScriptGraph& graph, NodeId id)
{
    auto it = std::lower_bound(graph.nodes.begin(), graph.nodes.end(), id, [](auto node, auto id)
    {
        return node.second < id;
    });

    return it == graph.nodes.end() ? -1 : std::distance(graph.nodes.begin(), it);
}

NodeCachePair getNode(ScriptGraph& graph, NodeId id)
{
    auto r = lookup(graph, id);
    return {graph.nodes[r].first, graph.cache[r].first};
}

ConstNodeCachePair getNode(const ScriptGraph& graph, NodeId id)
{
    auto r = lookup(graph, id);
    return {graph.nodes[r].first, graph.cache[r].first};
}

NodeId addNode(ScriptGraph& graph, const Node& node)
{
    NodeId id = graph.nodes.empty() ? 0 : graph.nodes.back().second + 1;

    graph.nodes.emplace_back(node, id);
    graph.cache.emplace_back(std::nullopt, id);

    boost::add_edge(id, id, graph.structure);

    for(auto &argument: node.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument))
        {
            auto callee = std::get<NodeId>(argument);
            if(lookup(graph, callee) != -1)
                boost::add_edge(std::get<NodeId>(argument), id, graph.structure);
        }
    }

    return id;
}

void editNode([[maybe_unused]]ScriptGraph& graph, [[maybe_unused]]NodeId id, [[maybe_unused]]const Node& newNode)
{
    auto r = lookup(graph, id);

    if(r != -1)
    {
        Node& node = graph.nodes[r].first;
        node = newNode;

        boost::remove_vertex(id, graph.structure);

        for (auto &argument: node.m_logic.m_argument)
        {
            if (std::holds_alternative<NodeId>(argument))
            {
                auto callee = std::get<NodeId>(argument);
                if (lookup(graph, callee) != -1)
                    boost::add_edge(std::get<NodeId>(argument), id, graph.structure);
            }
        }
        boost::add_edge(id, id, graph.structure);
    }
}

void removeNode([[maybe_unused]]ScriptGraph& graph, [[maybe_unused]]NodeId id)
{
    auto r = lookup(graph, id);
    graph.nodes.erase(graph.nodes.begin() + r);
    graph.cache.erase(graph.cache.begin() + r);
    boost::remove_vertex(id, graph.structure);
}