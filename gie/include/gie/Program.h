//
// Created by alex on 12/3/18.
//

#ifndef GIE_LIBRARY_PROGRAM_H
#define GIE_LIBRARY_PROGRAM_H

#include <gie/ScriptGraph/ScriptGraph.h>
#include <gie/Node.h>
#include <gie/PythonContext.h>
#include <gie/Result.h>
#include <gie/Error.h>

#include <vector>

class Program
{
public:
    Program() = default;
    Program(const Program&) = default;
    Program(Program&&) = default;

    MaybeError<std::vector<ExecutionInterfaceError>> run();

    NodeId addNode(std::string name, Arguments);

    template<typename Editor>
    MaybeError<NodeInterfaceError> editNode(NodeId id, Editor&& editor)
    {
        auto node = ::getNode(m_graph, id);

        if(node)
            editor(*(node->node));
        else return node.error();

        return ::updateNode(m_graph, id);
    }

    MaybeError<NodeInterfaceError> editNode(NodeId, ArgumentId argumentId, ArgumentValue);
    MaybeError<NodeInterfaceError> removeNode(NodeId);
    Expected<const Node*, NodeInterfaceError> getNode(NodeId id) const;
    Expected<std::optional<Value>, NodeInterfaceError> getCache(NodeId id) const;

    void addResult(std::string tag, NodeId);
    void editResult(std::string tag, NodeId);
    void removeResult(std::string tag);

    void import(const std::string& name, const std::string& path);

    auto& context() { return m_pythonContext; }
    const auto& context() const { return m_pythonContext; }

private:
    ScriptGraph m_graph;
    PythonContext m_pythonContext;
};


#endif //GIE_LIBRARY_PROGRAM_H
