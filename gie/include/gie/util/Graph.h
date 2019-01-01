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
    template<typename T>
    class Graph
    {
    private:
        struct Edge;
        class NeighboursProxy;
    public:
        using NodeId = std::size_t;

        void addEdge(NodeId from, NodeId to);
        void removeEdge(NodeId from, NodeId to);

        NodeId addNode(const T &nodeData);
        NodeId addNode(T &&nodeData);
        void removeNode(NodeId nodeId);

        T& getNode(NodeId id);
        std::vector<NodeId> getNodes();


        NeighboursProxy getNeighbours(NodeId sourceNode);

    public:
        class NeighboursIterator: std::iterator<
                std::forward_iterator_tag,
                Graph::NodeId,
                std::size_t,
                const Graph::NodeId *,
                Graph::NodeId
        >
        {
        private:
            NeighboursIterator(Graph::Edge *, std::vector<Edge> *);
        public:
            NeighboursIterator() = default;
            NeighboursIterator(const NeighboursIterator &) = default;
            NeighboursIterator(NeighboursIterator &&) noexcept = default;

            NeighboursIterator& operator++();

            bool operator==(const NeighboursIterator &other);
            bool operator!=(const NeighboursIterator &other);

            reference operator*() const;

        private:
            Graph::Edge *m_current;
            std::vector<Graph::Edge> *m_edges;
            friend NeighboursProxy;
        };

    private:
        class NeighboursProxy
        {
        private:
            NeighboursProxy(Edge *, std::vector<Edge> *);

        public:
            NeighboursProxy() = delete;

            NeighboursIterator begin();
            NeighboursIterator end();
        private:
            Graph::Edge *m_begin;
            std::vector<Graph::Edge> *m_edges;

            friend Graph;
        };

    private:
        std::vector<std::optional<T>> m_nodes;
        std::stack<NodeId> m_freeNodePositions;

    private:
        struct Edge
        {
            NodeId from, to;

            size_t next;
            size_t index;
        };

        std::vector<Edge> m_edges;
        std::stack<size_t> m_freeEdgePositions;
        std::unordered_map<NodeId, size_t> m_index;

        friend NeighboursIterator;
        friend NeighboursProxy;

    };
};

#include "Graph.inl"

#endif //GIE_LIBRARY_UTIL_GRAPH_H
