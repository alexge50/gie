//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_UTIL_GRAPH_H
#define GIE_LIBRARY_UTIL_GRAPH_H

#include <cstddef>

#include <unordered_map>
#include <vector>
#include <stack>

namespace util
{
    class NeighboursIterator;
    class NeighboursProxy;

    class Graph
    {
    public:
        using NodeId = std::size_t;

        void addEdge(NodeId from, NodeId to);
        void removeEdge(NodeId from, NodeId to);

        void removeNode(NodeId node);

        const NeighboursProxy getNeighbours(NodeId sourceNode);

    private:
        struct Edge
        {
            NodeId from, to;

            size_t next;
            size_t index;
        };

        std::vector<Edge> m_edges;
        std::stack<size_t> m_free;
        std::unordered_map<NodeId, size_t> m_index;

        friend NeighboursIterator;
        friend NeighboursProxy;
    };

    class NeighboursIterator: std::iterator<
            std::forward_iterator_tag,
            Graph::NodeId,
            std::size_t,
            const Graph::NodeId *,
            Graph::NodeId
    >
    {
    private:
        NeighboursIterator(Graph::Edge *, std::vector<Graph::Edge> *);
    public:
        NeighboursIterator& operator++();

        bool operator==(const NeighboursIterator &other);
        bool operator!=(const NeighboursIterator &other);

        reference operator*() const;

    private:
        Graph::Edge *m_current;
        std::vector<Graph::Edge> *m_edges;
        friend NeighboursProxy;
    };

    class NeighboursProxy
    {
    private:
        NeighboursProxy(Graph::Edge *, std::vector<Graph::Edge> *);

    public:
        NeighboursProxy() = delete;

        NeighboursIterator begin();
        NeighboursIterator end();
    private:
        Graph::Edge *m_begin;
        std::vector<Graph::Edge> *m_edges;

        friend Graph;
    };

};


#endif //GIE_LIBRARY_UTIL_GRAPH_H
