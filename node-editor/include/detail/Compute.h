#ifndef NODE_EDITOR_COMPUTE_H
#define NODE_EDITOR_COMPUTE_H

#include <algorithm>

inline void compute_type(NodeEditor& node_editor, NodeTypeId id)
{
    const StylingConfig& config = node_editor.styling_config;
    const float row_real_height = config.row_height + config.row_padding * 2.f;

    const NodeType& node_type = node_editor.graph.node_types.at(id);
    NodeTypeCompute& node_type_compute = node_editor.graph.node_types_computed[id];

    float width = 3 * config.margin_padding + 2 * node_editor.font->compute_bounding_box(node_type.name, config.text_height).x;

    for(const auto& port: node_type.output_ports)
    {
        width = std::max(width, 3 * config.margin_padding + 2 * node_editor.font->compute_bounding_box(port.name, config.text_height).x);
    }

    for(const auto& port: node_type.input_ports)
    {
        width = std::max(width, 3 * config.margin_padding + 2 * node_editor.font->compute_bounding_box(port.name, config.text_height).x);

        if(auto text_box = std::get_if<PortWidgets::TextBox>(&port.widget))
        {
            width = std::max(width, 3 * config.margin_padding + 2 * node_editor.font->compute_max_bounding_box(text_box->max_text_length, config.text_height).x);
        }
    }

    node_type_compute.size = {
            width,
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

    auto offset = node_type_compute.header_position.y + config.header_height / 2.f;

    node_type_compute.output_port_positions.resize(node_type.output_ports.size());
    for(size_t i = 0; i < node_type.output_ports.size(); i++)
    {
        node_type_compute.output_port_positions[i] = glm::vec2 {
                +size.x / 2.f,
                offset + row_real_height / 2.f
        };
        offset += row_real_height;
    }

    //offset = node_type_compute.header_position.y + config.header_height / 2.f;
    node_type_compute.input_port_positions.resize(node_type.input_ports.size());
    for(size_t i = 0; i < node_type.input_ports.size(); i++)
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

inline void compute_focus_stack(NodeEditor& node_editor)
{
    for(const auto [id, _]: node_editor.graph.nodes)
    {
        if(std::count(node_editor.focus_stack.begin(), node_editor.focus_stack.end(), id) == 0)
            node_editor.focus_stack.push_back(id);
    }
}

#endif //NODE_EDITOR_COMPUTE_H
