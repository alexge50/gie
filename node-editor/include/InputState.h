#ifndef NODE_EDITOR_INPUTSTATE_H
#define NODE_EDITOR_INPUTSTATE_H

#include <variant>
#include <unordered_set>
#include <glm/vec2.hpp>

struct NodeDrag
{
    glm::vec2 begin_position;
};

struct ConnectionDrag
{
    Port source_port;
    glm::vec2 source_position;
    glm::vec2 destination_position;
};

struct SelectDrag
{
    glm::vec2 begin_corner;
    glm::vec2 current_corner;
};

struct ViewDrag
{
    glm::vec2 begin_position;
};

struct NoDrag {};

using DisabledPorts = std::unordered_set<Port, PortHasher>;

struct InputState
{
    std::variant<NoDrag, ViewDrag, SelectDrag, ConnectionDrag, NodeDrag> drag_state = NoDrag{};
    std::unordered_set<NodeId> selected_nodes = {};
    DisabledPorts disabled_ports = {};
};

#endif //NODE_EDITOR_INPUTSTATE_H
