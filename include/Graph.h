#ifndef NODE_EDITOR_GRAPH_H
#define NODE_EDITOR_GRAPH_H

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using NodeId = std::int32_t;
using PortId = std::int32_t;

struct Node
{
    std::vector<std::string> input_ports;
    std::vector<std::string> output_ports;
    std::string node_name;

    glm::vec2 position;
    glm::vec3 color;
    glm::vec3 outline_color;
};

struct Connection
{
    struct Port
    {
        NodeId node_id;
        PortId port_id;
    } output_port, input_port;
};

struct Graph
{
    std::unordered_map<NodeId, Node> nodes;
    std::vector<Connection> connections;
};

#endif //NODE_EDITOR_GRAPH_H
