//
// Created by alex on 7/7/19.
//

#ifndef GIE_NODETYPEMANAGER_H
#define GIE_NODETYPEMANAGER_H


#include <gie/Argument.h>
#include <gie/Type.h>
#include <gie/PythonContext.h>
#include <gie/NodeType.h>

#include <optional>

class NodeTypeManager
{
public:
    explicit NodeTypeManager(const PythonContext& pythonContext):
            m_pythonContext{pythonContext}
    {}

    const NodeType* nodeType(NodeTypeId) const;
    const NodeType* nodeType(const Symbol&);
    const NodeType* nodeType(const std::string& name);

    std::optional<NodeTypeId> getId(const std::string& name);

private:
    std::vector<NodeType> m_nodeTypes;
    std::unordered_map<std::string, NodeTypeId> m_nameToId;
    const PythonContext& m_pythonContext;
};


#endif //GIE_NODETYPEMANAGER_H
