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
#include <gie/Error.h>

#include <Expected.h>

#include <boost/python.hpp>

Expected<Value, ExecutionInterfaceError> executeNode(const PythonContext&, const Node& node);

MaybeError<ExecutionInterfaceError> executeNode(const PythonContext&, ScriptGraph& graph, NodeId nodeId);
MaybeError<std::vector<ExecutionInterfaceError>> executeGraph(const PythonContext&, ScriptGraph& graph);

#endif //GIE_LIBRARY_EXECUTE_H
