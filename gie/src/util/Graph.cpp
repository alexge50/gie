//
// Created by alex on 12/1/18.
//

#include <gie/util/Graph.h>

#include "gie/util/Graph.h"


void util::Graph::addEdge(NodeId from, NodeId to)
{
    Edge toInsert = {from, to, m_index[from]};

    Edge *place;
    if(m_free.empty())
    {
        place = m_free.top();
        m_free.pop();

        *place = toInsert;
    }
    else
    {
        m_edges.push_back(toInsert);
        place = &m_edges[m_edges.size() - 1];
    }

    m_index[from] = place;
}

void util::Graph::removeEdge(NodeId from, NodeId to)
{
    Edge **source = nullptr;

    if(auto it = m_index.find(from); it != m_index.end())
        source = &it->second;
    else return;

    while(*source != nullptr)
    {
        if((*source)->to == to)
        {
            m_free.push(*source);
            *source = (*source)->next;
        }

        source = &((*source)->next);
    }
}

void util::Graph::removeNode(util::Graph::NodeId node)
{
    for(const auto &[key, _]: m_index)
        removeEdge(key, node);
}

const util::NeighboursProxy util::Graph::getNeighbours(util::Graph::NodeId sourceNode)
{
    if(auto it = m_index.find(sourceNode); it != m_index.end())
        return util::NeighboursProxy(it->second);
    else return util::NeighboursProxy(nullptr);
}

util::NeighboursIterator::NeighboursIterator(util::Graph::Edge* pCurrent)
{
    m_current = pCurrent;
}

bool util::NeighboursIterator::operator!=(const util::NeighboursIterator &other)
{
    return !(*this == other);
}

util::NeighboursIterator &util::NeighboursIterator::operator++()
{
    m_current = m_current->next;
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

util::NeighboursProxy::NeighboursProxy(util::Graph::Edge *begin)
{
    m_begin = begin;
}

util::NeighboursIterator util::NeighboursProxy::begin()
{
    return util::NeighboursIterator(m_begin);
}

util::NeighboursIterator util::NeighboursProxy::end()
{
    return util::NeighboursIterator(nullptr);
}
