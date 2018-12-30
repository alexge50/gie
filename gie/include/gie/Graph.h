//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_GRAPH_H
#define GIE_LIBRARY_GRAPH_H

#include "Node.h"

#include "util/Graph.h"

#include <unordered_map>
#include <optional>
#include <vector>
#include <stack>

class Graph
{
public:

    NodeId addNode(const Node &node);
    void removeNode(NodeId id);
    std::tuple<Node&, Value&> getNode(NodeId id);

    std::vector<NodeId> getNodes();

    auto getCallees(NodeId id)
    {
        return m_nodeUsageGraph.getNeighbours(id);
    }

    void removeCallee(NodeId id, NodeId target)
    {
        m_nodeUsageGraph.removeEdge(id, target);
    }

    void addCallee(NodeId id, NodeId target)
    {
        m_nodeUsageGraph.addEdge(id, target);
    }

private:
    std::vector<std::optional<Node>> m_nodes;
    std::vector<std::optional<Value>> m_cache;
    std::stack<NodeId> m_freePositions;

    void addConnection(NodeId user, NodeId used);
private:
    util::Graph m_nodeUsageGraph;
};


#endif //GIE_LIBRARY_GRAPH_H
