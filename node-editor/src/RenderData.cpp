#include <RenderData.h>
#include <NodeEditor.h>
#include <Render.h>

#include <glm/gtx/color_space.hpp>

static const float OUTLINE_Z_LOCATION = 2.f;
static const float BASE_Z_LOCATION = 0.f;
static const float PORT_Z_LOCATION = 3.f;
static const float PORT_OUTLINE_Z_LOCATION = 4.f;
static const float HEADER_Z_LOCATION = 1.f;
static const float TEXT_Z_LOCATION = 4.f;
static const float STRIDE_Z_LOCATION = 5.f;
static const float CONNECTION_Z_LOCATION = 1.f;

static const float TEXT_BOX_BACKGROUND_Z_LOCATION = 1.f;
static const float TEXT_BOX_OUTLINE_Z_LOCATION = 2.f;
static const float TEXT_BOX_TEXT_Z_LOCATION = 2.f;
static const float TEXT_BOX_CURSOR_Z_LOCATION = 3.f;

static void compute_render_data_widget(
        RenderData& cache,
        const StylingConfig& config,
        const Font& font,
        const Widgets::TextBoxState& state,
        CenteredBox box,
        int order,
        bool active)
{
    cache.quads.push_back(RenderData::Quad{
            glm::vec3(box.center, order * STRIDE_Z_LOCATION + TEXT_BOX_BACKGROUND_Z_LOCATION),
            glm::vec3(box.size, 1.f),
            glm::vec4{config.text_box_widget_background_color, 1.f},
    });

    cache.quad_outlines.push_back(RenderData::QuadOutline{
            glm::vec3(box.center, order * STRIDE_Z_LOCATION + TEXT_BOX_OUTLINE_Z_LOCATION),
            glm::vec3(box.size, 1.f),
            glm::vec4{active ? config.text_box_widget_outline_active_color : (state.invalid ? config.text_box_widget_outline_invalid_color : config.text_box_widget_outline_color), 1.f}
    });

    float max_height = font.max_height(config.text_height);
    float max_sub_line = font.max_sub_line(config.text_height);
    cache.stencil_texts.push_back(RenderData::StencilText{
            glm::vec3{state.text_box.center - glm::vec2{state.text_box.size.x / 2.f + state.view_position, -max_height / 2.f + max_sub_line}, order * STRIDE_Z_LOCATION + TEXT_BOX_TEXT_Z_LOCATION},
            glm::vec4{config.text_box_widget_text_color, 1.f},
            state.text,
            config.text_height,
            compute_bounding_box(state.text_box.center, state.text_box.size)
    });

    if(active)
    {
        float cursor_position = font.compute_bounding_box(
            std::string_view{state.text.data(), std::min(state.text.size(), state.cursor_position)},
            config.text_height
        ).x;

        cache.quads.emplace_back(RenderData::Quad{
            .position = glm::vec3{state.text_box.center.x + cursor_position - state.text_box.size.x / 2.f - state.view_position, state.text_box.center.y, order * STRIDE_Z_LOCATION + TEXT_BOX_CURSOR_Z_LOCATION},
            .size = glm::vec3{config.text_box_cursor_width, config.text_height, 0.f},
            .color = glm::vec4{1.f, 1.f, 1.f, 1.f}
        });
    }
}

static void compute_render_data_widget(
        RenderData& cache,
        const StylingConfig& config,
        const Font& font,
        const Widgets::ColorPickerState& state,
        CenteredBox box,
        int order,
        bool active)
{
    if(state.port_widget_box)
    {
        cache.quads.push_back(RenderData::Quad{
                glm::vec3(box.center, order * STRIDE_Z_LOCATION + TEXT_BOX_BACKGROUND_Z_LOCATION),
                glm::vec3(box.size, 1.f),
                glm::vec4{state.color, 1.f},
        });

        cache.quad_outlines.push_back(RenderData::QuadOutline{
                glm::vec3(box.center, order * STRIDE_Z_LOCATION + TEXT_BOX_OUTLINE_Z_LOCATION),
                glm::vec3(box.size, 1.f),
                glm::vec4{active ? config.text_box_widget_outline_active_color : config.text_box_widget_outline_color, 1.f}
        });
    }

    if(state.popup)
    {
        cache.quads.push_back(RenderData::Quad{
                glm::vec3(state.popup->box.center, order * STRIDE_Z_LOCATION + BASE_Z_LOCATION),
                glm::vec3(state.popup->box.size, 1.f),
                glm::vec4{config.node_background_color, 1.f},
        });

        cache.quad_outlines.push_back(RenderData::QuadOutline{
                glm::vec3(state.popup->box.center, order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION),
                glm::vec3(state.popup->box.size, 1.f),
                glm::vec4{active ? config.node_selected_outline_color : config.node_outline_color, 1.f}
        });

        cache.circle_color_space.push_back(RenderData::CircleColorSpace {
                glm::vec3{state.popup->color_wheel.center, order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION},
                glm::vec3{state.popup->color_wheel.size, 1.f},
                glm::hsvColor(state.color).z
        });

        cache.monochrome_gradient.push_back(RenderData::MonochromeGradient {
                glm::vec3{state.popup->luminance_bar.center, order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION},
                glm::vec3{state.popup->luminance_bar.size, 1.f}
        });

        float max_height = font.max_height(config.text_height);
        float max_sub_line = font.max_sub_line(config.text_height);
        cache.texts.push_back(RenderData::Text{
                glm::vec3{
                    state.popup->box.center.x - state.popup->box.size.x / 2.f + config.color_picker_margin_padding,
                    state.popup->channel_input_area[0].center.y + max_height / 2.f - max_sub_line,
                    order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION
                    },
                glm::vec4{config.text_box_widget_text_color, 1.f},
                "r:",
                config.text_height,
        });

        cache.texts.push_back(RenderData::Text{
                glm::vec3{
                        state.popup->box.center.x - state.popup->box.size.x / 2.f + config.color_picker_margin_padding,
                        state.popup->channel_input_area[1].center.y + max_height / 2.f - max_sub_line,
                        order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION
                },
                glm::vec4{config.text_box_widget_text_color, 1.f},
                "g:",
                config.text_height,
        });

        cache.texts.push_back(RenderData::Text{
                glm::vec3{
                        state.popup->box.center.x - state.popup->box.size.x / 2.f + config.color_picker_margin_padding,
                        state.popup->channel_input_area[2].center.y + max_height / 2.f - max_sub_line,
                        order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION
                },
                glm::vec4{config.text_box_widget_text_color, 1.f},
                "b:",
                config.text_height,
        });

        int active_text_box = state.popup->active_text_box ? *state.popup->active_text_box : -1;

        compute_render_data_widget(cache, config, font, state.popup->channel_text_box[0], state.popup->channel_input_area[0], order, active_text_box == 0);
        compute_render_data_widget(cache, config, font, state.popup->channel_text_box[1], state.popup->channel_input_area[1], order, active_text_box == 1);
        compute_render_data_widget(cache, config, font, state.popup->channel_text_box[2], state.popup->channel_input_area[2], order, active_text_box == 2);
    }
}

static void compute_render_data_widget(
        RenderData& cache,
        const StylingConfig& config,
        const Font& font,
        const Widgets::None& state,
        CenteredBox box,
        int order,
        bool active)
{}

RenderData compute_render_data(const NodeEditor& node_editor)
{
    RenderData cache;

    cache.quads.reserve(2 * node_editor.graph.nodes.size());
    cache.quad_outlines.reserve(node_editor.graph.nodes.size());
    for(const auto& [node_id, node]: node_editor.state.node_state)
    {
        cache.quads.push_back(RenderData::Quad{
               glm::vec3(node.position, node.order * STRIDE_Z_LOCATION),
               glm::vec3(node.size, 1.f),
               glm::vec4{node_editor.styling_config.node_background_color, 1.f}
        });

        glm::vec3 header_position;
        glm::vec3 header_size;

        cache.quads.push_back(RenderData::Quad{
            header_position = glm::vec3(node.header_position, node.order * STRIDE_Z_LOCATION + HEADER_Z_LOCATION),
            header_size = glm::vec3(node.header_size, 1.f),
            glm::vec4{node.color, 1.f},
        });

        {
            float max_sub_line = node_editor.font->max_sub_line(node_editor.styling_config.text_height);
            float max_height = node_editor.font->max_height(node_editor.styling_config.text_height);

            glm::vec2 text_position =
                    glm::vec2{header_position.x, header_position.y} + glm::vec2{-header_size.x / 2.f, max_height / 2.f - max_sub_line} +
                    glm::vec2{node_editor.styling_config.margin_padding, 0.f};

            cache.texts.push_back(RenderData::Text{
                    glm::vec3{text_position, node.order * STRIDE_Z_LOCATION + TEXT_Z_LOCATION},
                    node_editor.styling_config.text_color,
                    node.title,
                    node_editor.styling_config.text_height
            });
        }

        cache.quad_outlines.push_back(RenderData::QuadOutline{
                glm::vec3(node.position, node.order * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION),
                glm::vec3(node.size, 1.f),
                glm::vec4 {
                        node.selected ? node_editor.styling_config.node_selected_outline_color : node_editor.styling_config.node_outline_color,
                        1.f
                }
        });
    }

    for(const auto& [port_id, port]: node_editor.state.port_state)
    {

        cache.circles.push_back(RenderData::Circle{
            glm::vec3(port.position, port.order * STRIDE_Z_LOCATION + PORT_Z_LOCATION),
            node_editor.styling_config.port_radius,
            port.disabled ? node_editor.styling_config.disabled_port_color : port.color,
        });

        cache.circle_outlines.push_back(RenderData::CircleOutline{
            glm::vec3(port.position, port.order * STRIDE_Z_LOCATION + PORT_OUTLINE_Z_LOCATION),
            node_editor.styling_config.port_radius,
            node_editor.styling_config.node_outline_color
        });

        glm::vec2 text_size = node_editor.font->compute_bounding_box(port.label, node_editor.styling_config.text_height);
        float text_x = port_id.type == Port::Type::INPUT ?
                       node_editor.styling_config.margin_padding:
                       -node_editor.styling_config.margin_padding - text_size.x;

        float max_sub_line = node_editor.font->max_sub_line(node_editor.styling_config.text_height);
        float max_height = node_editor.font->max_height(node_editor.styling_config.text_height);

        cache.texts.push_back(RenderData::Text{
            glm::vec3{port.position + glm::vec2{text_x, max_height / 2.f - max_sub_line}, port.order * STRIDE_Z_LOCATION + TEXT_Z_LOCATION},
            node_editor.styling_config.text_color,
            port.label,
            node_editor.styling_config.text_height
        });
    }

    cache.lines.reserve(node_editor.graph.connections.size() + 1);

    for(const auto& connection: node_editor.graph.connections)
    {

        const glm::vec2 source_position =
                node_editor.state.port_state.at(Port{connection.input_port.node_id, connection.input_port.port_id, Port::Type::INPUT}).position;
        const glm::vec2 destination_position =
                node_editor.state.port_state.at(Port{connection.output_port.node_id, connection.output_port.port_id, Port::Type::OUTPUT}).position;

        cache.lines.push_back(RenderData::Line{
            glm::vec3(source_position, CONNECTION_Z_LOCATION),
            glm::vec3(destination_position, CONNECTION_Z_LOCATION),
            node_editor.styling_config.connection_color
        });
    }

    for(const auto& [_, state_data_binded]: node_editor.state.widget_state)
    {
        const auto& state_data = state_data_binded;

        std::visit([&](const auto& state) {
            compute_render_data_widget(cache, node_editor.styling_config, *node_editor.font, state, state_data.box, state_data.order, state_data.active);
        }, state_data.state);
    }

    if(auto connection_drag = std::get_if<ConnectionDrag>(&node_editor.state.input_state.drag_state))
    {
        auto connection_z = static_cast<float>(node_editor.state.focus_stack.size() + 1) * STRIDE_Z_LOCATION + 1.f;
        cache.lines.push_back(RenderData::Line{
                glm::vec3(connection_drag->source_position, connection_z),
                glm::vec3(connection_drag->destination_position, connection_z),
                node_editor.styling_config.connection_color
        });
    }
    else if(auto select_drag = std::get_if<SelectDrag>(&node_editor.state.input_state.drag_state))
    {
        auto select_box_z = static_cast<float>(node_editor.state.focus_stack.size() + 1) * STRIDE_Z_LOCATION + 1.f;

        cache.quads.emplace_back(RenderData::Quad{
                .position = glm::vec3{(select_drag->begin_corner + select_drag->current_corner) / 2.f, select_box_z},
                .size = glm::vec3{glm::abs(select_drag->begin_corner - select_drag->current_corner), 0.f},
                .color = node_editor.styling_config.select_rectangle_color
        });

        cache.quad_outlines.emplace_back(RenderData::QuadOutline{
                .position = glm::vec3{(select_drag->begin_corner + select_drag->current_corner) / 2.f, select_box_z + 1.f},
                .size = glm::vec3{glm::abs(select_drag->begin_corner - select_drag->current_corner), 0.f},
                .color = node_editor.styling_config.select_rectangle_outline_color
        });
    }

    cache.camera = node_editor.camera;
    cache.line_width = node_editor.styling_config.node_outline_width;
    cache.background_color = node_editor.styling_config.grid_background;
    cache.foreground_color = node_editor.styling_config.grid_foreground;

    return cache;
}