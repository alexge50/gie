#ifndef NODE_EDITOR_COMPUTENODE_H
#define NODE_EDITOR_COMPUTENODE_H

#include <algorithm>

inline void compute_node(NodeEditor& node_editor, NodeId id)
{
    const StylingConfig& config = node_editor.styling_config;
    const float row_real_height = config.row_height + config.row_padding * 2.f;
    Node& node = node_editor.graph.nodes[id];

    node_editor.graph.nodes_computed[id].size = {
            100.f,
            std::max(
                    node.input_ports.size() + node.output_ports.size(),
                    static_cast<std::size_t>(1)
            ) * row_real_height +
            config.header_height
    };

    const auto& size = node_editor.graph.nodes_computed[id].size;

    node_editor.graph.nodes_computed[id].header_position = glm::vec2 {
            0.f,
            -size.y / 2.f + config.header_height / 2.f
    } + node.position;

    auto offset = config.header_height;

    node_editor.graph.nodes_computed[id].output_port_positions.resize(node.output_ports.size());
    for(int i = 0; i < node.output_ports.size(); i++)
    {
        node_editor.graph.nodes_computed[id].output_port_positions[i] = glm::vec2 {
                +size.x / 2.f,
                offset + row_real_height / 2.f
        } + node.position;
        offset += row_real_height;
    }

    node_editor.graph.nodes_computed[id].input_port_positions.resize(node.input_ports.size());
    for(int i = 0; i < node.input_ports.size(); i++)
    {
        node_editor.graph.nodes_computed[id].input_port_positions[i] = glm::vec2 {
                -size.x / 2.f,
                offset + row_real_height / 2.f
        } + node.position;
        offset += row_real_height;
    }
}

#endif //NODE_EDITOR_COMPUTENODE_H
