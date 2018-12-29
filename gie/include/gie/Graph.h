//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_GRAPH_H
#define GIE_LIBRARY_GRAPH_H

#include "Node.h"

#include "util/Graph.h"

#include <unordered_map>
#include <vector>
#include <stack>

class Graph
{
public:

    NodeId addNode(const Node &node);
    void removeNode(NodeId id);

private:
    std::vector<Node> m_nodes;
    std::stack<NodeId> m_freePositions;

    void addConnection(NodeId user, NodeId used);
private:
    util::Graph m_nodeUsageGraph;
};


#endif //GIE_LIBRARY_GRAPH_H