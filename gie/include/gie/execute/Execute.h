//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_EXECUTE_H
#define GIE_LIBRARY_EXECUTE_H

#include <gie/Value.h>
#include <gie/Graph.h>
#include <gie/Node.h>

#include <gie/PythonContext.h>

#include <boost/python.hpp>

Value executeNode(const PythonContext &context, Graph &graph, NodeId nodeId);
std::vector<std::pair<NodeId, bool>> calculateRuntimeOrder(Graph &graph);

std::vector<Value> executeGraph(const PythonContext &context, Graph &graph);

#endif //GIE_LIBRARY_EXECUTE_H
