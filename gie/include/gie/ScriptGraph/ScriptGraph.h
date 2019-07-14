//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_SCENEGRAPH_H
#define GIE_LIBRARY_SCENEGRAPH_H

#include <gie/Node.h>
#include <gie/Error.h>

#include <MaybeError.h>
#include <Expected.h>

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
    Node* node;
    std::optional<Value>* cache;
};

struct ConstNodeCachePair
{
    const Node* node;
    const std::optional<Value>* cache;
};

Expected<NodeCachePair, NodeInterfaceError> getNode(ScriptGraph&, NodeId);
Expected<ConstNodeCachePair, NodeInterfaceError> getNode(const ScriptGraph&, NodeId);

NodeId addNode(ScriptGraph&, const Node&);
MaybeError<NodeInterfaceError> removeNode(ScriptGraph&, NodeId);
MaybeError<NodeInterfaceError> editNode(ScriptGraph&, NodeId, ArgumentId argumentId, ArgumentValue);
MaybeError<NodeInterfaceError> updateNode(ScriptGraph&, NodeId);

void addResult(ScriptGraph&, std::string tag, NodeId);
void editResult(ScriptGraph&, std::string tag, NodeId);
void removeResult(ScriptGraph&, std::string tag);

#endif //GIE_LIBRARY_SCENEGRAPH_H
