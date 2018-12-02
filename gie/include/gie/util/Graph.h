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

            Edge* next;
        };

        std::vector<Edge> m_edges;
        std::stack<Edge*> m_free;
        std::unordered_map<NodeId, Edge*> m_index;

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
        explicit NeighboursIterator(Graph::Edge*);
    public:
        NeighboursIterator& operator++();

        bool operator==(const NeighboursIterator &other);
        bool operator!=(const NeighboursIterator &other);

        reference operator*() const;

    private:
        Graph::Edge *m_current;
        friend NeighboursProxy;
    };

    class NeighboursProxy
    {
    private:
        explicit NeighboursProxy(Graph::Edge *);

    public:
        NeighboursProxy() = delete;

        NeighboursIterator begin();
        NeighboursIterator end();
    private:
        Graph::Edge *m_begin;

        friend Graph;
    };

};


#endif //GIE_LIBRARY_UTIL_GRAPH_H
