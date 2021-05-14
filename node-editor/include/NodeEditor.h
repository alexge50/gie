#ifndef NODE_EDITOR_NODEEDITOR_H
#define NODE_EDITOR_NODEEDITOR_H

#include <Graph.h>
#include <Camera.h>
#include <InputState.h>
#include <GraphCache.h>
#include <Font.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <functional>

struct StylingConfig
{
    float text_height = 12.f;
    float row_height = 14.f;
    float row_padding = 1.f;
    float header_height = 14.f;
    glm::vec4 text_color = glm::vec4{1.f, 1.f, 1.f, 1.f};

    glm::vec3 grid_background = {0.137f, 0.137f, 0.137f};
    glm::vec3 grid_foreground = {0.1f, 0.1f, 0.1f};

    glm::vec4 select_rectangle_color = {1.f, 1.f, 1.f, 0.3f};
    glm::vec4 select_rectangle_outline_color = {1.f, 1.f, 1.f, 0.7f};

    glm::vec3 node_background_color = {0.24f, 0.24f, 0.24f};
    glm::vec3 node_outline_color = {0.3f, 0.3f, 0.3f};
    glm::vec3 node_selected_outline_color = {1.f, 1.f, 1.f};
    float node_outline_width = 1.f;

    glm::vec3 disabled_port_color = {0.24f, 0.24f, 0.24f};
    float port_radius = 10.f;

    glm::vec3 connection_color = {1.f, 1.f, 1.f};
    float connection_width = 2.f;
};

struct NodeEditor
{
    Graph graph{};
    Camera camera{};

    InputState input_state;
    std::vector<NodeId> focus_stack;

    StylingConfig styling_config{};
    std::function<DisabledPorts(Port)> compute_disabled_ports;
    Font* font;
};

void compute(NodeEditor&);

#endif //NODE_EDITOR_NODEEDITOR_H
