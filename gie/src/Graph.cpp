//
// Created by alexddd on 12/1/18.
//

#include <gie/Graph.h>
#include <gie/util/Graph.h>


NodeId Graph::addNode(const Node &node)
{
    if(m_freePositions.empty())
    {
        m_nodes.push_back(node);
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
    if(id == m_nodes.size() - 1)
        m_nodes.pop_back();
    else m_freePositions.push(id);

    m_nodeUsageGraph.removeNode(id);
}

void Graph::addConnection(NodeId user, NodeId used)
{
    m_nodeUsageGraph.addEdge(user, used);
}

Node &Graph::getNode(NodeId id)
{
    return m_nodes[id];
}
