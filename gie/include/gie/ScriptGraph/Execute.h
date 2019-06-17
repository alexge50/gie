//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_EXECUTE_H
#define GIE_LIBRARY_EXECUTE_H

#include <gie/Value.h>
#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Node.h>
#include <gie/PythonContext.h>
#include <gie/Result.h>

#include <boost/python.hpp>

std::optional<Value> executeNode(const Node& node);

void executeNode(ScriptGraph& graph, NodeId nodeId);
std::vector<Result> executeGraph(ScriptGraph& graph); //TODO: doesn't do any checking (typing or NoArgument)

#endif //GIE_LIBRARY_EXECUTE_H
