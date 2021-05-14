#include <GraphCache.h>
#include <NodeEditor.h>
#include <Render.h>

#include <detail/Zip.h>

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

        cache.node_headers.push_back(NodeHeaderCache{
            glm::vec3(node.position + node_type_computed.header_position, z * STRIDE_Z_LOCATION + HEADER_Z_LOCATION),
            glm::vec3(node_type_computed.size.x, node_editor.styling_config.header_height, 1.f),
            node_type.color,
            node_type.name
        });

        cache.node_outlines.push_back(NodeOutlineCache{
                glm::vec3(node.position, z * STRIDE_Z_LOCATION + OUTLINE_Z_LOCATION),
                glm::vec3(node_type_computed.size, 1.f),
                selected
        });

        for(const auto&[position, port]:
                ConstZipObject{node_type_computed.input_port_positions, node_type.input_ports})
        {
            cache.ports.push_back(PortCache{
                glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_Z_LOCATION),
                port.color,
            });

            cache.port_outline.push_back(PortOutlineCache{
                    glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_OUTLINE_Z_LOCATION),
                    selected
            });
        }

        for(const auto&[position, port]:
                ConstZipObject{node_type_computed.output_port_positions, node_type.output_ports})
        {
            cache.ports.push_back(PortCache{
                    glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_Z_LOCATION),
                    port.color,
            });

            cache.port_outline.push_back(PortOutlineCache{
                    glm::vec3(position + node.position, z * STRIDE_Z_LOCATION + PORT_OUTLINE_Z_LOCATION),
                    selected
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