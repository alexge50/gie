#include <GraphCache.h>
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

GraphCache compute_graph_cache(const NodeEditor& node_editor)
{
    GraphCache cache;

    cache.nodes.reserve(node_editor.graph.nodes.size());
    cache.node_outlines.reserve(node_editor.graph.nodes.size());
    cache.node_headers.reserve(node_editor.graph.nodes.size());
    auto z = static_cast<float>(node_editor.focus_stack.size());
    for(const auto node_id: node_editor.focus_stack)
    {
        const Node& node = node_editor.graph.nodes.at(node_id);
        const NodeTypeCompute& node_type_computed = node_editor.graph.node_types_computed.at(node.node_type);
        const NodeType& node_type = node_editor.graph.node_types.at(node.node_type);
        bool selected = node_editor.input_state.selected_nodes.contains(node_id);

        cache.nodes.push_back(NodeCache{
               glm::vec3(node.position, z * STRIDE_Z_LOCATION),
               glm::vec3(node_type_computed.size, 1.f)
        });

        glm::vec3 header_position;
        glm::vec3 header_size;

        cache.node_headers.push_back(NodeHeaderCache{
            header_position = glm::vec3(node.position + node_type_computed.header_position, z * STRIDE_Z_LOCATION + HEADER_Z_LOCATION),
            header_size = glm::vec3(node_type_computed.size.x, node_editor.styling_config.header_height, 1.f),
            node_type.color,
        });

        {
            glm::vec2 text_size = node_editor.font->compute_bounding_box(node_type.name,
                                                                         node_editor.styling_config.text_height);
            glm::vec2 text_position =
                    glm::vec2{header_position.x, header_position.y} + glm::vec2{-header_size.x, +text_size.y} / 2.f +
                    glm::vec2{node_editor.styling_config.margin_padding, 0.f};

            cache.texts.push_back(TextCache{
                    glm::vec3{text_position, z * STRIDE_Z_LOCATION + TEXT_Z_LOCATION},
                    node_editor.styling_config.text_color,
                    node_type.name,
                    node_editor.styling_config.text_height
            });
        }

        cache.node_outlines.push_back(NodeOutlineCache{
                glm::vec3(node.position, z * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION),
                glm::vec3(node_type_computed.size, 1.f),
                selected
        });

        for(const auto&[position, port]:
                ConstZipObject{node_type_computed.input_port_positions, node_type.input_ports})
        {
            bool disabled = false;
            if(auto source_port = std::get_if<ConnectionDrag>(&node_editor.input_state.drag_state))
            {
                disabled = source_port->source_port.type == Port::Type::INPUT;
            }

            cache.ports.push_back(PortCache{
                glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_Z_LOCATION),
                disabled ? node_editor.styling_config.disabled_port_color : port.color,
            });

            cache.port_outlines.push_back(PortOutlineCache{
                    glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_OUTLINE_Z_LOCATION),
                    selected
            });

            glm::vec2 text_size = node_editor.font->compute_bounding_box(port.name, node_editor.styling_config.text_height);

            cache.texts.push_back(TextCache{
                glm::vec3{position + node.position + glm::vec2{node_editor.styling_config.margin_padding, text_size.y / 2.f}, z * STRIDE_Z_LOCATION + TEXT_Z_LOCATION},
                node_editor.styling_config.text_color,
                port.name,
                node_editor.styling_config.text_height
            });
        }

        for(const auto&[position, port]:
                ConstZipObject{node_type_computed.output_port_positions, node_type.output_ports})
        {
            bool disabled = false;
            if(auto source_port = std::get_if<ConnectionDrag>(&node_editor.input_state.drag_state))
            {
                disabled = source_port->source_port.type == Port::Type::OUTPUT;
            }

            cache.ports.push_back(PortCache{
                    glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_Z_LOCATION),
                    disabled ? node_editor.styling_config.disabled_port_color : port.color,
            });

            cache.port_outlines.push_back(PortOutlineCache{
                    glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_OUTLINE_Z_LOCATION),
                    selected
            });

            glm::vec2 text_size = node_editor.font->compute_bounding_box(port.name, node_editor.styling_config.text_height);

            cache.texts.push_back(TextCache{
                    glm::vec3{position + node.position + glm::vec2{-node_editor.styling_config.margin_padding - text_size.x, text_size.y / 2.f}, z * STRIDE_Z_LOCATION + TEXT_Z_LOCATION},
                    node_editor.styling_config.text_color,
                    port.name,
                    node_editor.styling_config.text_height
            });
        }

        z--;
    }

    cache.connections.reserve(node_editor.graph.connections.size() + 1);
    for(const auto& connection: node_editor.graph.connections)
    {
        const glm::vec2 source_position = node_editor.graph.nodes.at(connection.input_port.node_id).position +
                                  node_editor.graph.node_types_computed.at(
                                          node_editor.graph.nodes.at(connection.input_port.node_id).node_type
                                  ).input_port_positions[connection.input_port.port_id];
        const glm::vec2 destination_position = node_editor.graph.nodes.at(connection.output_port.node_id).position +
                                  node_editor.graph.node_types_computed.at(
                                          node_editor.graph.nodes.at(connection.output_port.node_id).node_type
                                  ).output_port_positions[connection.output_port.port_id];

        cache.connections.push_back(ConnectionCache{
            glm::vec3(source_position, CONNECTION_Z_LOCATION),
            glm::vec3(destination_position, CONNECTION_Z_LOCATION),
        });
    }

    if(auto connection_drag = std::get_if<ConnectionDrag>(&node_editor.input_state.drag_state))
    {
        auto connection_z = static_cast<float>(node_editor.focus_stack.size() + 1) * STRIDE_Z_LOCATION + 1.f;
        cache.connections.push_back(ConnectionCache{
                glm::vec3(connection_drag->source_position, connection_z),
                glm::vec3(connection_drag->destination_position, connection_z),
        });
    }
    else if(auto select_drag = std::get_if<SelectDrag>(&node_editor.input_state.drag_state))
    {
        cache.select_box = SelectBoxCache{
                {select_drag->begin_corner, select_drag->current_corner}
        };
    }

    cache.camera = node_editor.camera;

    return cache;
}