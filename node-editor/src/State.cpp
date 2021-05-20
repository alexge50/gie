#include <State.h>
#include <NodeEditor.h>

static void compute_type(NodeEditorState& node_editor_state, const StylingConfig& config, const Font& font, const NodeType& node_type, NodeTypeId node_type_id)
{
    const float row_real_height = config.row_height + config.row_padding * 2.f;

    float width = 3 * config.margin_padding + 2 * font.compute_bounding_box(node_type.name, config.text_height).x;

    for(const auto& port: node_type.output_ports)
    {
        width = std::max(width, 3 * config.margin_padding + 2 * font.compute_bounding_box(port.name, config.text_height).x);
    }

    for(const auto& port: node_type.input_ports)
    {
        width = std::max(width, 3 * config.margin_padding + 2 * font.compute_bounding_box(port.name, config.text_height).x);

        if(auto text_box = std::get_if<Widgets::TextBox>(&port.widget))
        {
            width = std::max(width, 3 * config.margin_padding + 2 * font.compute_max_bounding_box(text_box->max_text_length, config.text_height).x);
        }
    }

    glm::vec2 size = {
            width,
            std::max(
                    node_type.input_ports.size() + node_type.output_ports.size(),
                    static_cast<std::size_t>(1)
            ) * row_real_height +
            config.header_height
    };

    node_editor_state.node_type_state[node_type_id] =  {
            .size = size,
            .color = node_type.color,
            .header_position = {
                    0.f,
                    -size.y / 2.f + config.header_height / 2.f
            },
            .header_size = {
                    size.x,
                    config.header_height
            },
            .title = node_type.name,
            .input_port_count = static_cast<int>(node_type.input_ports.size()),
            .output_port_count = static_cast<int>(node_type.output_ports.size()),
    };

    const auto& node_type_state = node_editor_state.node_type_state[node_type_id];

    auto offset = node_type_state.header_position.y + config.header_height / 2.f;

    for(int i = 0; i < node_type_state.output_port_count; i++)
    {
        node_editor_state.node_type_output_port_state[Port{node_type_id, i, Port::Type::OUTPUT}] = {
                .color = node_type.output_ports[i].color,
                .position =  glm::vec2 {
                        +size.x / 2.f,
                        offset + row_real_height / 2.f
                },
                .label = node_type.output_ports[i].name,
                .radius = config.port_radius
        };

        offset += row_real_height;
    }

    for(int i = 0; i < node_type_state.input_port_count; i++)
    {
        glm::vec2 widget_size = glm::vec2 {
                (size.x - 3 * config.margin_padding) / 2.f,
                config.row_height
        };

        glm::vec2 widget_position =
                glm::vec2{size.x / 2.f - config.margin_padding, offset + row_real_height / 2.f} -
                glm::vec2{widget_size.x / 2.f, 0.f};

        node_editor_state.node_type_input_port_state[Port{node_type_id, i, Port::Type::INPUT}] = {
                .color = node_type.input_ports[i].color,
                .position =  glm::vec2 {
                        -size.x / 2.f,
                        offset + row_real_height / 2.f
                },
                .label = node_type.input_ports[i].name,
                .radius = config.port_radius,
                .widget_box = CenteredBox{widget_position, widget_size},
                .port_widget = node_type.input_ports[i].widget
        };

        offset += row_real_height;
    }
}

static void compute_node(NodeEditorState& node_editor_state, const StylingConfig& config, const Node& node, NodeId node_id)
{
    const auto& node_type = node_editor_state.node_type_state.at(node.node_type);
    auto& node_state = node_editor_state.node_state[node_id];

    node_state = NodeState{
        .position = node.position,
        .size = node_type.size,
        .header_position = node.position + node_type.header_position,
        .header_size = node_type.header_size,
        .color = node_type.color,
        .title = node_type.title,
        .order = node_editor_state.order[node_id],
        .selected = node_state.selected,
    };

    node_editor_state.interactive_element_state.push_back(InteractiveElementState{
            .element = InteractiveElementState::Node{node_id},
            .area = {CenteredBox{node_state.position, node_state.size}},
            .order = node_state.order
    });


    for(int i = 0; i < node_type.input_port_count; i++)
    {
        const auto& port_type = node_editor_state.node_type_input_port_state.at(Port{node.node_type, i, Port::Type::INPUT});
        auto& port_state = node_editor_state.port_state[Port{node_id, i, Port::Type::INPUT}];

         port_state = PortState {
            .position = node.position + port_type.position,
            .color = port_type.color,
            .label = port_type.label,
            .radius = port_type.radius,
            .order = node_state.order,
            .disabled = port_state.disabled,
        };

        node_editor_state.interactive_element_state.push_back(InteractiveElementState{
            .element = InteractiveElementState::Port{Port{node_id, i, Port::Type::INPUT}},
            .area = {Circle{port_state.position, port_state.radius}},
            .order = node_state.order
        });

        if(!std::holds_alternative<Widgets::None>(port_type.port_widget))
        {
            auto& widget_state = node_editor_state.widget_state[Port{node_id, i, Port::Type::INPUT}];

            widget_state = WidgetDataState {
                .state = widget_state.state,
                .box = port_type.widget_box,
                .order = node_state.order,
                .active = widget_state.active
            };

            widget_state.box.center += node.position;

            node_editor_state.interactive_element_state.push_back(InteractiveElementState{
                    .element = InteractiveElementState::Widget{Port{node_id, i, Port::Type::INPUT}},
                    .area = widget_state.box,
                    .order = node_state.order
            });
        }
    }

    for(int i = 0; i < node_type.output_port_count; i++)
    {
        const auto& port_type = node_editor_state.node_type_output_port_state.at(Port{node.node_type, i, Port::Type::OUTPUT});
        auto& port_state = node_editor_state.port_state[Port{node_id, i, Port::Type::OUTPUT}];

        port_state = PortState {
                .position = node.position + port_type.position,
                .color = port_type.color,
                .label = port_type.label,
                .radius = port_type.radius,
                .order = node_state.order,
                .disabled = port_state.disabled,
        };

        node_editor_state.interactive_element_state.push_back(InteractiveElementState{
                .element = InteractiveElementState::Port{Port{node_id, i, Port::Type::OUTPUT}},
                .area = {Circle{port_state.position, port_state.radius}},
                .order = node_state.order
        });
    }

}

void compute_state(const NodeEditor& node_editor, NodeEditorState& node_editor_state)
{
    for(const auto& [node_type_id, node_type]: node_editor.graph.node_types)
    {
        compute_type(node_editor_state, node_editor.styling_config, *node_editor.font, node_type, node_type_id);
    }

    for(const auto& [node_id, node]: node_editor.graph.nodes)
    {
        if(!node_editor_state.order.contains(node_id))
        {
            node_editor_state.focus_stack.push_back(node_id);

        }
    }

    for(const auto& node_id: node_editor_state.focus_stack)
    {
        if(!node_editor.graph.nodes.contains(node_id))
        {
            // remove from focus stack and order
        }
    }

    {
        int order = static_cast<int>(node_editor_state.focus_stack.size());
        for(const auto& node_id: node_editor_state.focus_stack)
        {
            node_editor_state.order[node_id] = order;
            order--;
        }
    }

    node_editor_state.interactive_element_state.clear();
    for(const auto& [node_id, node]: node_editor.graph.nodes)
    {
        compute_node(node_editor_state, node_editor.styling_config, node, node_id);
    }

    std::sort(node_editor_state.interactive_element_state.begin(), node_editor_state.interactive_element_state.end(),
              [](const auto& lhs, const auto& rhs) -> bool{
        if(lhs.order > rhs.order)
            return true;
        else if(lhs.order == rhs.order)
            return std::get_if<InteractiveElementState::Node>(&rhs.element);
        else return false;
    });
}