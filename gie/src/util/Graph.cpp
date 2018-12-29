//
// Created by alex on 12/1/18.
//

#include <gie/util/Graph.h>

#include "gie/util/Graph.h"


void util::Graph::addEdge(NodeId from, NodeId to)
{
    Edge toInsert = {from, to, m_index[from], 0};

    size_t place;
    if(m_free.empty())
    {
        place = m_free.top();
        m_free.pop();

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

void util::Graph::removeEdge(NodeId from, NodeId to)
{
    size_t *source = nullptr;

    if(auto it = m_index.find(from); it != m_index.end())
        source = &(it->second);
    else return;

    while(*source != 0)
    {
        if(m_edges[*source].to == to)
        {
            m_free.push(*source);
            *source = m_edges[*source].next;
        }

        source = &m_edges[*source].next;
    }
}

void util::Graph::removeNode(util::Graph::NodeId node)
{
    for(const auto &[key, _]: m_index)
        removeEdge(key, node);
}

util::NeighboursProxy util::Graph::getNeighbours(util::Graph::NodeId sourceNode)
{
    if(auto it = m_index.find(sourceNode); it != m_index.end())
        return util::NeighboursProxy(&m_edges[it->second], &m_edges);
    else return util::NeighboursProxy(nullptr, &m_edges);
}

util::NeighboursIterator::NeighboursIterator(Graph::Edge *pCurrent, std::vector<Graph::Edge> *edges)
{
    m_current = pCurrent;
    m_edges = edges;
}

bool util::NeighboursIterator::operator!=(const util::NeighboursIterator &other)
{
    return !(*this == other);
}

util::NeighboursIterator &util::NeighboursIterator::operator++()
{
    m_current = &((*m_edges)[m_current->next]);
    return *this;
}

bool util::NeighboursIterator::operator==(const util::NeighboursIterator &other)
{
    return m_current == other.m_current;
}

util::Graph::NodeId util::NeighboursIterator::operator*() const
{
    return m_current->to;
}

util::NeighboursProxy::NeighboursProxy(Graph::Edge *begin, std::vector<Graph::Edge> *edges)
{
    m_begin = begin;
    m_edges = edges;
}

util::NeighboursIterator util::NeighboursProxy::begin()
{
    return util::NeighboursIterator(m_begin, m_edges);
}

util::NeighboursIterator util::NeighboursProxy::end()
{
    return util::NeighboursIterator(nullptr, m_edges);
}
