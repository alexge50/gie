//
// Created by alex on 12/3/18.
//

#ifndef GIE_LIBRARY_PROGRAM_H
#define GIE_LIBRARY_PROGRAM_H

#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Node.h>
#include <gie/PythonContext.h>
#include <gie/Result.h>

#include <vector>

// TODO: change caching strategy
// TODO: check memory lifetime

// TODO: Add functions, graphs with set arguments and an output

// TODO: Add serialization and deserialization

class Program
{
public:
    Program() = default;
    Program(const Program &) = default;
    Program(Program &&) = default;

    std::vector<Result> run();
    // TODO: strong typing for NodeId
    NodeId addNode(const Node &node);
    void editNode(NodeId, const Node &node); // TODO: [refactoring] node editing should be done on a per argument basis
    void removeNode(NodeId);
    const Node& getNode(NodeId id) const;

    void addResult(std::string tag, NodeId); // TODO: tag should be an id instead
    void editResult(std::string tag, NodeId);
    void removeResult(std::string tag);

    void import(const std::string &module);

    auto& context() { return m_pythonContext; }
    const auto& context() const { return m_pythonContext; }

private:
    ScriptGraph m_graph;
    PythonContext m_pythonContext;
};


#endif //GIE_LIBRARY_PROGRAM_H
