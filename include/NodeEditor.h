#ifndef NODE_EDITOR_NODEEDITOR_H
#define NODE_EDITOR_NODEEDITOR_H

#include <Graph.h>
#include <glm/vec2.hpp>

#include <variant>

struct StylingConfig
{
    float text_height = 12.f;
    float row_height = 14.f;
    float row_padding = 1.f;
    float header_height = 14.f;

    glm::vec3 grid_background = {0.137f, 0.137f, 0.137f};
    glm::vec3 grid_foreground = {0.1f, 0.1f, 0.1f};

    glm::vec3 node_background_color = {0.24f, 0.24f, 0.24f};
};

struct NodeDrag
{
    NodeId node;
    glm::vec2 begin_position;
};

struct ConnectionDrag
{

};

struct SelectDrag
{
    glm::vec2 begin_corner;
};

struct ViewDrag
{
    glm::vec2 begin_position;
};

struct NoDrag {};

struct NodeEditor
{
    Graph graph;

    float zoom = 2.f;
    glm::vec2 camera_position;
    std::variant<NoDrag, ViewDrag, SelectDrag, ConnectionDrag, NodeDrag> drag_state;

    StylingConfig styling_config;
};

void compute(NodeEditor&);

#endif //NODE_EDITOR_NODEEDITOR_H
