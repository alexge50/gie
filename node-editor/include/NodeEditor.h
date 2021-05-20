#ifndef NODE_EDITOR_NODEEDITOR_H
#define NODE_EDITOR_NODEEDITOR_H

#include <Graph.h>
#include <Camera.h>
#include <InputState.h>
#include <RenderData.h>
#include <State.h>
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
    float header_height = 20.f;
    float margin_padding = 8.0f;
    float bottom_padding = 6.f;
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

    glm::vec3 text_box_widget_background_color = {0.28f, 0.28f, 0.28f};
    glm::vec3 text_box_widget_outline_color = {0.36f, 0.36f, 0.36f};
    glm::vec3 text_box_widget_outline_active_color = {1.f, 1.f, 1.f};
    glm::vec3 text_box_widget_text_color = {1.f, 1.f, 1.f};
};

struct NodeEditor
{
    Graph graph{};
    Camera camera{};

    NodeEditorState state;

    StylingConfig styling_config{};
    Font* font = nullptr;
};

void compute(NodeEditor&);

#endif //NODE_EDITOR_NODEEDITOR_H
