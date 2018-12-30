//
// Created by alex on 12/1/18.
//

#include <gie/Graph.h>
#include <gie/util/Graph.h>


NodeId Graph::addNode(const Node &node)
{
    if(m_freePositions.empty())
    {
        m_nodes.emplace_back(node);
        return m_nodes.size() - 1;
    }
    else
    {
        NodeId id = m_freePositions.top();
        m_freePositions.pop();

        m_nodes[id] = node;
        return id;
    }
}

void Graph::removeNode(NodeId id)
{
    m_nodes[id] = std::nullopt;
    if(id == m_nodes.size() - 1)
        m_nodes.pop_back();
    else m_freePositions.push(id);

    m_nodeUsageGraph.removeNode(id);
}

void Graph::addConnection(NodeId user, NodeId used)
{
    m_nodeUsageGraph.addEdge(user, used);
}

std::tuple<Node&, Value&> Graph::getNode(NodeId id)
{
    return {m_nodes[id].value(), m_cache[id].value()};
}

std::vector<NodeId> Graph::getNodes()
{
    std::vector<NodeId> nodes;

    for(int i = 0; i < m_nodes.size(); i++)
        if(m_nodes[i].has_value())
            nodes.emplace_back(i);

    return nodes;
}
