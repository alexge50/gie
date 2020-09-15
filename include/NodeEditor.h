#ifndef NODE_EDITOR_NODEEDITOR_H
#define NODE_EDITOR_NODEEDITOR_H

#include <Graph.h>
#include <glm/vec2.hpp>

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

struct NodeEditor
{
    Graph graph;

    float zoom = 2.f;
    glm::vec2 camera_position;
    StylingConfig styling_config;
};

void compute(NodeEditor&);

#endif //NODE_EDITOR_NODEEDITOR_H
