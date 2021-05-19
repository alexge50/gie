#include <RenderData.h>
#include <NodeEditor.h>
#include <Render.h>

#include <detail/Zip.h>

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
               node_editor.styling_config.node_background_color
        });

        glm::vec3 header_position;
        glm::vec3 header_size;

        cache.quads.push_back(RenderData::Quad{
            header_position = glm::vec3(node.header_position, node.order * STRIDE_Z_LOCATION + HEADER_Z_LOCATION),
            header_size = glm::vec3(node.header_size, 1.f),
            node.color,
        });

        {
            glm::vec2 text_size = node_editor.font->compute_bounding_box(node.title,
                                                                         node_editor.styling_config.text_height);
            glm::vec2 text_position =
                    glm::vec2{header_position.x, header_position.y} + glm::vec2{-header_size.x, +text_size.y} / 2.f +
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
                node.selected ? node_editor.styling_config.node_selected_outline_color : node_editor.styling_config.node_outline_color
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

        cache.texts.push_back(RenderData::Text{
            glm::vec3{port.position + glm::vec2{text_x, text_size.y / 2.f}, port.order * STRIDE_Z_LOCATION + TEXT_Z_LOCATION},
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

    for(const auto& [port, text_widget_state]: node_editor.state.text_box_state)
    {
        const auto& metadata = node_editor.state.widget_metadata.at(port);

        cache.quads.push_back(RenderData::Quad{
                glm::vec3(metadata.box.center, metadata.order * STRIDE_Z_LOCATION + TEXT_BOX_BACKGROUND_Z_LOCATION),
                glm::vec3(metadata.box.size, 1.f),
                node_editor.styling_config.node_background_color,
        });

        cache.quad_outlines.push_back(RenderData::QuadOutline{
                glm::vec3(metadata.box.center, metadata.order * STRIDE_Z_LOCATION + TEXT_BOX_OUTLINE_Z_LOCATION),
                glm::vec3(metadata.box.size, 1.f),
                node_editor.styling_config.node_outline_color
        });

        cache.stencil_texts.push_back(RenderData::StencilText{
                glm::vec3{metadata.box.center - glm::vec2{metadata.box.size.x / 2.f, -node_editor.styling_config.text_height / 2.f}, metadata.order * STRIDE_Z_LOCATION + TEXT_BOX_TEXT_Z_LOCATION},
                node_editor.styling_config.text_color,
                text_widget_state.data,
                node_editor.styling_config.text_height,
                compute_bounding_box(metadata.box.center, metadata.box.size)
        });
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
        cache.select_box = RenderData::SelectBox{
                {select_drag->begin_corner, select_drag->current_corner},
                node_editor.styling_config.select_rectangle_color,
                node_editor.styling_config.select_rectangle_outline_color,
        };
    }

    cache.camera = node_editor.camera;
    cache.line_width = node_editor.styling_config.node_outline_width;
    cache.background_color = node_editor.styling_config.grid_background;
    cache.foreground_color = node_editor.styling_config.grid_foreground;

    return cache;
}