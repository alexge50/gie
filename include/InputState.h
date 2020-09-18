#ifndef NODE_EDITOR_INPUTSTATE_H
#define NODE_EDITOR_INPUTSTATE_H

#include <variant>
#include <unordered_set>

struct NodeDrag
{
    glm::vec2 begin_position;
};

struct ConnectionDrag
{

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

struct InputState
{
    std::variant<NoDrag, ViewDrag, SelectDrag, ConnectionDrag, NodeDrag> drag_state = NoDrag{};
    std::unordered_set<NodeId> selected_nodes = {};
};

#endif //NODE_EDITOR_INPUTSTATE_H
