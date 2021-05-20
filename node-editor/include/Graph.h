#ifndef NODE_EDITOR_GRAPH_H
#define NODE_EDITOR_GRAPH_H

#include "detail/BoundingArea.h"
#include <Widget.h>

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

static bool operator==(const Port& lhs, const Port& rhs)
{
    return lhs.node_id == rhs.node_id &&
           lhs.port_id == rhs.port_id &&
           lhs.type == rhs.type;
}

struct PortHasher
{
    size_t operator()(const Port& port) const
    {
        size_t r = 0;

        std::hash<int32_t> hasher;

        r ^= hasher(port.node_id) + 0x9e3779b9 + (r << 6) + (r >> 2);
        r ^= hasher(port.port_id) + 0x9e3779b9 + (r << 6) + (r >> 2);
        r ^= hasher(static_cast<int>(port.type)) + 0x9e3779b9 + (r << 6) + (r >> 2);

        return r;
    }
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
        Widget widget;
    };

    std::vector<Port> input_ports;
    std::vector<Port> output_ports;
    std::string name;
    glm::vec3 color;
};

struct NodeTypeCompute
{
    std::vector<glm::vec2> input_port_positions;
    std::vector<CenteredBox> input_widget_boxes;
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
