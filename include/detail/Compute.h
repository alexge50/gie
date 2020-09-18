#ifndef NODE_EDITOR_COMPUTE_H
#define NODE_EDITOR_COMPUTE_H

#include <algorithm>

inline void compute_type(NodeEditor& node_editor, NodeTypeId id)
{
    const StylingConfig& config = node_editor.styling_config;
    const float row_real_height = config.row_height + config.row_padding * 2.f;

    const NodeType& node_type = node_editor.graph.node_types.at(id);
    NodeTypeCompute& node_type_compute = node_editor.graph.node_types_computed[id];

    node_type_compute.size = {
            100.f,
            std::max(
                    node_type.input_ports.size() + node_type.output_ports.size(),
                    static_cast<std::size_t>(1)
            ) * row_real_height +
            config.header_height
    };

    const auto& size = node_type_compute.size;

    node_type_compute.header_position = glm::vec2 {
            0.f,
            -size.y / 2.f + config.header_height / 2.f
    };

    auto offset = config.header_height;

    node_type_compute.output_port_positions.resize(node_type.output_ports.size());
    for(int i = 0; i < node_type.output_ports.size(); i++)
    {
        node_type_compute.output_port_positions[i] = glm::vec2 {
                +size.x / 2.f,
                offset + row_real_height / 2.f
        };
        offset += row_real_height;
    }

    node_type_compute.input_port_positions.resize(node_type.input_ports.size());
    for(int i = 0; i < node_type.input_ports.size(); i++)
    {
        node_type_compute.input_port_positions[i] = glm::vec2 {
                -size.x / 2.f,
                offset + row_real_height / 2.f
        };
        offset += row_real_height;
    }
}

static void compute_node(NodeEditor& node_editor, NodeId id)
{
    NodeCompute& node_compute = node_editor.graph.node_computed[id];
    const Node& node = node_editor.graph.nodes.at(id);
    const NodeTypeCompute& node_type_compute = node_editor.graph.node_types_computed.at(node.node_type);

    node_compute.bounding_box = compute_bounding_box(node.position, node_type_compute.size);
}

#endif //NODE_EDITOR_COMPUTE_H
