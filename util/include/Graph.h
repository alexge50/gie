//
// Created by alex on 7/14/19.
//

#ifndef GIE_GRAPH_H
#define GIE_GRAPH_H

template <typename NodeType>
class Graph
{
private:
    using NodeId = std::size_t;
    static const std::size_t InvalidNode = 0;

public:
    void addNode(const NodeType& key)
    {
        if(auto it = m_map.find(key); it == m_map.end())
        {
            m_graph.push_back({m_nodeIdCount, {}, {}});
            m_idMap.insert({m_nodeIdCount, key});
            m_map.insert({key, m_nodeIdCount});

            m_nodeIdCount++;
        }
    }

    void removeNode(const NodeType& key)
    {
        auto id = m_map[key];

        for(auto& neighbour: m_graph[id].in)
        {
            auto neighbourIndex = graphLookup(neighbour);
            removeNeighbour(m_graph[neighbourIndex].out, id);
        }

        for(auto& neighbour: m_graph[id].out)
        {
            auto neighbourIndex = graphLookup(neighbour);
            removeNeighbour(m_graph[neighbourIndex].in, id);
        }

        removeNode(id);
    }

    void addEdge(const NodeType& a, const NodeType& b)
    {
        addNode(a);
        addNode(b);

        auto aId = m_map[a];
        auto bId = m_map[b];
        auto& aNode = m_graph[graphLookup(aId)];
        auto& bNode = m_graph[graphLookup(bId)];

        addNeighbour(aNode.out, bId);
        addNeighbour(bNode.in, aId);
    }

    void removeEdge(const NodeType& a, const NodeType& b)
    {
        auto aIt = m_map.find(a);
        auto bIt = m_map.find(b);

        if(aIt == m_map.end() || bIt == m_map.end())
            return ;

        auto& aNode = m_graph[graphLookup(aIt->second)];
        auto& bNode = m_graph[graphLookup(bIt->second)];

        removeNeighbour(aNode.in, bIt->second);
        removeNeighbour(aNode.out, bIt->second);
        removeNeighbour(bNode.in, aIt->second);
        removeNeighbour(bNode.out, aIt->second);
    }

    template <typename Callback>
    void iterateInNeighbours(const NodeType& key, Callback&& callback)
    {
        for(auto neighbour: m_graph[graphLookup(m_map[key])].in)
            callback(m_idMap[neighbour]);
    }

    template <typename Callback>
    void iterateOutNeighbours(const NodeType& key, Callback&& callback)
    {
        for(auto neighbour: m_graph[graphLookup(m_map[key])].out)
            callback(m_idMap[neighbour]);
    }

    std::size_t inDegree(const NodeType& key)
    {
        return m_graph[graphLookup(m_map[key])].in.size();
    }

    std::size_t outDegree(const NodeType& key)
    {
        return m_graph[graphLookup(m_map[key])].out.size();
    }

private:
    void addNeighbour(std::vector<NodeId>& neighbour, NodeId id)
    {
        auto it = neighbour.begin();

        while(it != neighbour.end() && *it < id)
            it++;

        if(it == neighbour.end() || *it != id)
            neighbour.insert(it, id);
    }

    void removeNeighbour(std::vector<NodeId>& neighbour, NodeId id)
    {
        auto r = findInNeighbours(neighbour, id);

        if(r != NotFound)
            neighbour.erase(neighbour.begin() + r);
    }

    void removeNode(NodeId id)
    {
        auto r = graphLookup(id);

        if(r != NotFound)
            m_graph.erase(m_graph.begin() + r);
    }

private:
    static const std::size_t NotFound = -1;

    std::size_t graphLookup(NodeId id)
    {
        struct
        {
            std::size_t id;
        } shadowId{id};

        auto it = std::lower_bound(m_graph.begin(), m_graph.end(), shadowId, [](const auto& lhs, const auto& rhs)
        {
            return lhs.id < rhs.id;
        });

        return it == m_graph.end() ? NotFound : std::distance(m_graph.begin(), it);
    }

    std::size_t findInNeighbours(const std::vector<NodeId>& neighbours, NodeId id)
    {
        auto it = std::lower_bound(neighbours.begin(), neighbours.end(), id);
        return it == neighbours.end() ? NotFound : std::distance(neighbours.begin(), it);
    }

private:
    std::size_t m_nodeIdCount = 0;
    std::unordered_map<NodeType, NodeId> m_map;
    std::unordered_map<NodeId, NodeType> m_idMap;

    struct Node
    {
        NodeId id{};
        std::vector<NodeId> in;
        std::vector<NodeId> out;
    };

    std::vector<Node> m_graph;
};

#endif //GIE_GRAPH_H