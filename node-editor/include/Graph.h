#ifndef NODE_EDITOR_GRAPH_H
#define NODE_EDITOR_GRAPH_H

#include "detail/BoundingBox.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using NodeId = std::int32_t;
using PortId = std::int32_t;
using NodeTypeId = std::int32_t;

struct Node
{
    NodeTypeId node_type;

    glm::vec2 position;
};

struct Port
{
    enum class Type
    {
        INPUT,
        OUTPUT,
        NOT_APPLICABLE
    };

    NodeId node_id = -1;
    PortId port_id = -1;
    Type type = Type::NOT_APPLICABLE;
};

struct Connection
{
    Port output_port, input_port;
};

struct NodeCompute
{
    BoundingBox bounding_box;
};

struct NodeType
{
    struct Port
    {
        std::string name;
        glm::vec3 color;
    };

    std::vector<Port> input_ports;
    std::vector<Port> output_ports;
    std::string name;
    glm::vec3 color;
};

struct NodeTypeCompute
{
    std::vector<glm::vec2> input_port_positions;
    std::vector<glm::vec2> output_port_positions;

    glm::vec2 size;
    glm::vec2 header_position;
};

struct Graph
{
    std::unordered_map<NodeId, Node> nodes;
    std::unordered_map<NodeTypeId, NodeType> node_types;
    std::vector<Connection> connections;
    std::unordered_map<NodeId, NodeCompute> node_computed;
    std::unordered_map<NodeTypeId, NodeTypeCompute> node_types_computed;
};

#endif //NODE_EDITOR_GRAPH_H
