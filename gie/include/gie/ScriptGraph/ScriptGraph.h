//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_SCENEGRAPH_H
#define GIE_LIBRARY_SCENEGRAPH_H

#include <gie/Node.h>

#include <utility>
#include <optional>

struct ScriptGraph
{
    std::vector<std::pair<Node, NodeId>> nodes;
    std::vector<std::pair<std::optional<Value>, NodeId>> cache;
    std::vector<std::pair<std::string, NodeId>> results;
};

struct NodeCachePair
{
    Node& node;
    std::optional<Value>& cache;
};

struct ConstNodeCachePair
{
    const Node& node;
    const std::optional<Value>& cache;
};

NodeCachePair getNode(ScriptGraph&, NodeId);
ConstNodeCachePair getNode(const ScriptGraph&, NodeId);

NodeId addNode(ScriptGraph&, const Node&);
void removeNode(ScriptGraph&, NodeId);
void editNode(ScriptGraph&, NodeId, ArgumentId argumentId, ArgumentValue);
void updateNode(ScriptGraph&, NodeId);

void addResult(ScriptGraph&, std::string tag, NodeId);
void editResult(ScriptGraph&, std::string tag, NodeId);
void removeResult(ScriptGraph&, std::string tag);

#endif //GIE_LIBRARY_SCENEGRAPH_H
