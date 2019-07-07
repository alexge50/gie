//
// Created by alex on 7/7/19.
//

#include "gie/NodeTypeManager.h"
#include <gie/Node.h>

const NodeType* NodeTypeManager::nodeType(NodeTypeId id) const
{
    if(id < m_nodeTypes.size())
        return &m_nodeTypes[id];
    else return nullptr;
}

const NodeType* NodeTypeManager::nodeType(const Symbol& symbol)
{
    return nodeType(symbol.qualifiedName);
}

const NodeType* NodeTypeManager::nodeType(const std::string& name)
{
    auto it = m_nameToId.find(name);

    if(it != m_nameToId.end())
        return &m_nodeTypes[it->second];
    else
    {
        m_nodeTypes.push_back(fetchNodeType(m_pythonContext, name));
        auto id = m_nodeTypes.size() - 1;
        m_nameToId[name] = id;

        return &m_nodeTypes[id];
    }
}

std::optional<NodeTypeId> NodeTypeManager::getId(const std::string& name)
{
    if(nodeType(name) != nullptr)
        return m_nameToId[name];
    return std::nullopt;
}