//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_SCENEGRAPH_H
#define GIE_LIBRARY_SCENEGRAPH_H

#include <gie/Node.h>
#include <gie/util/Graph.h>

#include <utility>
#include <optional>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>

struct ScriptGraph
{
    std::vector<std::pair<Node, NodeId>> nodes;
    std::vector<std::pair<std::optional<Value>, NodeId>> cache;

    using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    graph structure;
};

struct NodeCachePair
{
    Node& node;
    std::optional<Value>& cache;
};

NodeCachePair getNode(ScriptGraph&, NodeId);

NodeId addNode(ScriptGraph&, const Node&);
void editNode(ScriptGraph&, NodeId, const Node&);
void removeNode(ScriptGraph&, NodeId);

#endif //GIE_LIBRARY_SCENEGRAPH_H
