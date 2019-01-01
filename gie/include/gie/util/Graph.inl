//
// Created by alex on 1/1/19.
//

#ifndef GIE_LIBRARY_GRAPH_INL
#define GIE_LIBRARY_GRAPH_INL

#include <util/Graph.h>

template<typename T>
void util::Graph<T>::addEdge(NodeId from, NodeId to)
{
    Edge toInsert = {from, to, m_index[from], 0};

    size_t place;
    if(m_freeEdgePositions.empty())
    {
        place = m_freeEdgePositions.top();
        m_freeEdgePositions.pop();

        toInsert.index = place;
        m_edges[place] = toInsert;
    }
    else
    {
        m_edges.push_back(toInsert);
        place = m_edges.size() - 1;
        m_edges.back().index = m_edges.size() - 1;
    }

    m_index[from] = place;
}

template<typename T>
void util::Graph<T>::removeEdge(NodeId from, NodeId to)
{
    size_t *source = nullptr;

    if(auto it = m_index.find(from); it != m_index.end())
        source = &(it->second);
    else return;

    while(*source != 0)
    {
        if(m_edges[*source].to == to)
        {
            m_freeEdgePositions.push(*source);
            *source = m_edges[*source].next;
        }

        source = &m_edges[*source].next;
    }
}

template<typename T>
NodeId util::Graph<T>::addNode(const T &nodeData)
{
    if(m_freeNodePositions.empty())
    {
        m_nodes.emplace_back(nodeData);
        return m_nodes.size() - 1;
    }
    else
    {
        NodeId id = m_freeNodePositions.top();
        m_freeNodePositions.pop();

        m_nodes[id] = nodeData;
        return id;
    }
}

template<typename T>
NodeId util::Graph<T>::addNode(T &&nodeData)
{
    if(m_freeNodePositions.empty())
    {
        m_nodes.emplace_back(nodeData);
        return m_nodes.size() - 1;
    }
    else
    {
        NodeId id = m_freeNodePositions.top();
        m_freeNodePositions.pop();

        m_nodes[id] = nodeData;
        return id;
    }
}

template<typename T>
void util::Graph<T>::removeNode(NodeId nodeId)
{
    m_nodes[nodeId] = std::nullopt;
    if(nodeId == m_nodes.size() - 1)
        m_nodes.pop_back();
    else m_freeNodePositions.push(nodeId);

    for(const auto &[key, _]: m_index)
        removeEdge(key, nodeId);
}

template<typename T>
NeighboursProxy util::Graph<T>::getNeighbours(NodeId sourceNode)
{
    if(auto it = m_index.find(sourceNode); it != m_index.end())
        return util::NeighboursProxy(&m_edges[it->second], &m_edges);
    else return util::NeighboursProxy(nullptr, &m_edges);
}


template<typename T>
util::Graph<T>::NeighboursIterator::NeighboursIterator(util::Graph<T>::Edge *pCurrent, std::vector<util::Graph<T>::Edge> *edges)
{
    m_current = pCurrent;
    m_edges = edges;
}

template<typename T>
util::Graph<T>::NeighboursIterator &util::Graph<T>::NeighboursIterator::operator++()
{
    m_current = &((*m_edges)[m_current->next]);
    return *this;
}

template<typename T>
bool util::Graph<T>::NeighboursIterator::operator==(const util::Graph<T>::NeighboursIterator &other)
{
    return m_current == other.m_current;
}

template<typename T>
bool util::Graph<T>::NeighboursIterator::operator!=(const util::Graph<T>::NeighboursIterator &other)
{
    return !(*this == other);
}

template<typename T>
unsigned long util::Graph<T>::NeighboursIterator::operator*() const
{
    return m_current->to;
}

template<typename T>
util::Graph<T>::NeighboursProxy::NeighboursProxy(util::Graph<T>::Edge *begin, std::vector<util::Graph<T>::Edge> *edges)
{
    m_begin = begin;
    m_edges = edges;
}

template<typename T>
util::Graph<T>::NeighboursIterator util::Graph<T>::NeighboursProxy::begin()
{
    return util::Graph<T>::NeighboursIterator(m_begin, m_edges);
}

template<typename T>
util::Graph::NeighboursIterator util::Graph<T>::NeighboursProxy::end() {
    return util::Graph<T>::NeighboursIterator(nullptr, m_edges);
}


#endif //GIE_LIBRARY_GRAPH_INL
