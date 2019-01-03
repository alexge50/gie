//
// Created by alex on 1/1/19.
//

#ifndef GIE_LIBRARY_GRAPH_INL
#define GIE_LIBRARY_GRAPH_INL

#include "Graph.h"

template<typename T>
void util::Graph<T>::addEdge(NodeId from, NodeId to)
{
    Edge toInsert = {from, to, m_index[from], 0};

    size_t place;
    if(!m_freeEdgePositions.empty())
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

    for([[maybe_unused]]const auto &[key, _]: m_index)
        removeEdge(key, nodeId);
}

template<typename T>
T &util::Graph<T>::getNode(NodeId id)
{
    return m_nodes[id].value();
}

template<typename T>
const T &util::Graph<T>::getNode(NodeId id) const
{
    return m_nodes[id].value();
}

template<typename T>
std::vector<typename util::Graph<T>::NodeId> util::Graph<T>::getNodes() const
{
    std::vector<util::Graph<T>::NodeId> nodes;

    for(unsigned int i = 0; i < m_nodes.size(); i++)
        if(m_nodes[i].has_value())
            nodes.emplace_back(i);

    return nodes;
}

template<typename T>
typename util::Graph<T>::NeighboursProxy util::Graph<T>::getNeighbours(NodeId sourceNode)
{
    if(auto it = m_index.find(sourceNode); it != m_index.end())
        return util::Graph<T>::NeighboursProxy(&m_edges[it->second], &m_edges);
    else return util::Graph<T>::NeighboursProxy(nullptr, &m_edges);
}

template<typename T>
const typename util::Graph<T>::ConstNeighboursProxy util::Graph<T>::getNeighbours(NodeId sourceNode) const
{
    if(auto it = m_index.find(sourceNode); it != m_index.end())
        return util::Graph<T>::ConstNeighboursProxy(&m_edges[it->second], &m_edges);
    else return util::Graph<T>::ConstNeighboursProxy(nullptr, &m_edges);
}

template<typename T>
util::Graph<T>::NeighboursIterator::NeighboursIterator(util::Graph<T>::Edge *pCurrent, std::vector<util::Graph<T>::Edge> *edges)
{
    m_current = pCurrent;
    m_edges = edges;
}

template<typename T>
typename util::Graph<T>::NeighboursIterator &util::Graph<T>::NeighboursIterator::operator++()
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
util::Graph<T>::ConstNeighboursIterator::ConstNeighboursIterator(const util::Graph<T>::Edge *pCurrent, const std::vector<util::Graph<T>::Edge> *edges)
{
    m_current = pCurrent;
    m_edges = edges;
}

template<typename T>
typename util::Graph<T>::ConstNeighboursIterator &util::Graph<T>::ConstNeighboursIterator::operator++()
{
    m_current = &((*m_edges)[m_current->next]);
    return *this;
}

template<typename T>
bool util::Graph<T>::ConstNeighboursIterator::operator==(const util::Graph<T>::ConstNeighboursIterator &other)
{
    return m_current == other.m_current;
}

template<typename T>
bool util::Graph<T>::ConstNeighboursIterator::operator!=(const util::Graph<T>::ConstNeighboursIterator &other)
{
    return !(*this == other);
}

template<typename T>
unsigned long util::Graph<T>::ConstNeighboursIterator::operator*() const
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
typename util::Graph<T>::NeighboursIterator util::Graph<T>::NeighboursProxy::begin()
{
    return util::Graph<T>::NeighboursIterator(m_begin, m_edges);
}

template<typename T>
typename util::Graph<T>::NeighboursIterator util::Graph<T>::NeighboursProxy::end()
{
    return util::Graph<T>::NeighboursIterator(nullptr, m_edges);
}

template<typename T>
util::Graph<T>::ConstNeighboursProxy::ConstNeighboursProxy(const util::Graph<T>::Edge *begin, const std::vector<util::Graph<T>::Edge> *edges)
{
    m_begin = begin;
    m_edges = edges;
}

template<typename T>
typename util::Graph<T>::ConstNeighboursIterator util::Graph<T>::ConstNeighboursProxy::begin() const
{
    return util::Graph<T>::ConstNeighboursIterator(m_begin, m_edges);
}

template<typename T>
typename util::Graph<T>::ConstNeighboursIterator util::Graph<T>::ConstNeighboursProxy::end() const
{
    return util::Graph<T>::ConstNeighboursIterator(nullptr, m_edges);
}

#endif //GIE_LIBRARY_GRAPH_INL
