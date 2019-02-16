//
// Created by alex on 2/16/19.
//

#include <gie/SceneGraph.h>

#include <optional>
#include <functional>
#include <type_traits>


static long lookup(ScriptGraph& graph, NodeId id)
{
    auto it = std::lower_bound(graph.nodes.begin(), graph.nodes.end(), id, [](auto node, auto id)
    {
        return node.second < id;
    });

    return it == graph.nodes.end() ? -1 : std::distance(graph.nodes.end(), it);
}

NodeId addNode(ScriptGraph& graph, const Node& node)
{
    NodeId id = graph.nodes.back().second + 1;

    graph.nodes.emplace_back(node, id);
    graph.cache.emplace_back(std::nullopt, id);

    for(auto &argument: node.m_logic.m_argument)
    {
        if(std::holds_alternative<NodeId>(argument.second))
        {
            auto callee = std::get<NodeId>(argument.second);
            if(lookup(graph, callee) != -1)
                boost::add_edge(std::get<NodeId>(argument.second), id, graph.structure);
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

        for (auto &argument: node.m_logic.m_argument) {
            if (std::holds_alternative<NodeId>(argument.second)) {
                auto callee = std::get<NodeId>(argument.second);
                if (lookup(graph, callee) != -1)
                    boost::add_edge(std::get<NodeId>(argument.second), id, graph.structure);
            }
        }
    }
}

void removeNode([[maybe_unused]]ScriptGraph& graph, [[maybe_unused]]NodeId id)
{
    auto r = lookup(graph, id);
    graph.nodes.erase(graph.nodes.begin() + r);
    graph.cache.erase(graph.cache.begin() + r);
    boost::remove_vertex(id, graph.structure);
}