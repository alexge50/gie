//
// Created by alex on 12/3/18.
//

#ifndef GIE_LIBRARY_PROGRAM_H
#define GIE_LIBRARY_PROGRAM_H

#include "gie/ScriptGraph/ScriptGraph.h"
#include "gie/Node.h"
#include "PythonContext.h"

#include <optional>

class Program
{
public:
    Program() = default;
    Program(const Program &) = default;
    Program(Program &&) = default;

    std::optional<Value> run();

    NodeId addNode(const Node &node);
    void editNode(NodeId, const Node &node);
    void removeNode(NodeId);
    const Node& getNode(NodeId id) const;

    void import(const std::string &module);

    auto& context() { return m_pythonContext; }
    const auto& context() const { return m_pythonContext; }

private:
    ScriptGraph m_graph;
    PythonContext m_pythonContext;
};


#endif //GIE_LIBRARY_PROGRAM_H
