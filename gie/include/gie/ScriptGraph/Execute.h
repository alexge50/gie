//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_EXECUTE_H
#define GIE_LIBRARY_EXECUTE_H

#include <gie/Value.h>
#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <boost/python.hpp>

std::optional<Value> executeNode(const Node& node);

Value executeNode(ScriptGraph& graph, NodeId nodeId);
std::vector<std::pair<NodeId, bool>> calculateRuntimeOrder(const ScriptGraph::graph& graph);

std::vector<std::pair<std::string, Value>> executeGraph(ScriptGraph& graph);

#endif //GIE_LIBRARY_EXECUTE_H
