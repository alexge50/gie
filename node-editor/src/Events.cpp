#include <Events.h>
#include <NodeEditor.h>
#include <detail/Compute.h>

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

static std::optional<NodeId> get_containing_node(const NodeEditor& node_editor, glm::vec2 point)
{
    auto node = std::find_if(
            node_editor.focus_stack.begin(),
            node_editor.focus_stack.end(),
            [&node_editor, &point](const auto& node) {
                const NodeCompute& node_compute = node_editor.graph.node_computed.at(node);
                return contains_point(node_compute.bounding_box, point);
            });

    if(node == node_editor.focus_stack.end())
        return std::nullopt;
    else return *node;
}

struct ContainingPort
{
    Port port;
    glm::vec2 position;
};

static std::optional<ContainingPort> get_containing_port(const NodeEditor& node_editor, glm::vec2 point)
{
    for(const auto& node_id: node_editor.focus_stack)
    {
        const Node& node = node_editor.graph.nodes.at(node_id);
        const NodeTypeCompute& node_type_compute = node_editor.graph.node_types_computed.at(node.node_type);

        for(size_t i = 0; i < node_type_compute.input_port_positions.size(); i++)
        {
            const auto& port = node_type_compute.input_port_positions[i];

            if(contains_point(Circle{node.position + port, node_editor.styling_config.port_radius}, point))
            {
                return ContainingPort {
                        Port {
                                node_id,
                                static_cast<PortId>(i),
                                Port::Type::INPUT
                        },
                        node.position + port
                };
            }
        }

        for(size_t i = 0; i < node_type_compute.output_port_positions.size(); i++)
        {
            const auto& port = node_type_compute.output_port_positions[i];

            if(contains_point(Circle{node.position + port, node_editor.styling_config.port_radius}, point))
            {
                return ContainingPort {
                        Port {
                                node_id,
                                static_cast<PortId>(i),
                                Port::Type::OUTPUT
                        },
                        node.position + port
                };
            }
        }
    }

    return std::nullopt;
}

std::optional<InputState::PortTextWidgetState> get_containing_widget(const NodeEditor& node_editor, glm::vec2 point)
{
    for(const auto& text_widget: node_editor.input_state.text_widget_state)
    {
        if(contains_point(compute_bounding_box(text_widget.state.box.center, text_widget.state.box.size), point))
        {
            return text_widget;
        }
    }

    return std::nullopt;
}

static void move_first_focus_stack(NodeEditor& node_editor, NodeId node)
{
    std::erase(node_editor.focus_stack, node);
    node_editor.focus_stack.insert(node_editor.focus_stack.begin(), node);
}

static std::optional<size_t> find_connection(NodeEditor& node_editor, Port port)
{
    for(size_t i = 0; i < node_editor.graph.connections.size(); i++)
    {
        if(port.type == Port::Type::INPUT)
        {
            if(node_editor.graph.connections[i].input_port.node_id == port.node_id &&
               node_editor.graph.connections[i].input_port.port_id == port.port_id)
            {
                return i;
            }
        }
        else if(port.type == Port::Type::OUTPUT)
        {
            if(node_editor.graph.connections[i].output_port.node_id == port.node_id &&
               node_editor.graph.connections[i].output_port.port_id == port.port_id)
            {
                return i;
            }
        }
    }

    return std::nullopt;
}

void process(NodeEditor& node_editor, const std::vector<InputEvent>& input, std::vector<EditorEvent>& editor_events)
{

    {
        int order = 0;
        for(const auto& node_id: node_editor.focus_stack)
        {
            const Node& node = node_editor.graph.nodes.at(node_id);
            const NodeTypeCompute& node_type_computed = node_editor.graph.node_types_computed.at(node.node_type);


            for(auto& text_widget_state: node_editor.input_state.text_widget_state)
            {
                if(text_widget_state.port.node_id == node_id)
                {
                    text_widget_state.order = order;
                    text_widget_state.state.box = node_type_computed.input_widget_boxes[text_widget_state.port.port_id];
                    text_widget_state.state.box.center += node.position;
                }
            }

            order++;
        }

        std::sort(node_editor.input_state.text_widget_state.begin(), node_editor.input_state.text_widget_state.end(),
                  [](const auto& lhs, const auto& rhs){
            return lhs.order < rhs.order;
        });
    }


    for(const auto& event: input)
    {
        std::visit(
                overloaded {
                    [&](const InputEvents::Click& click) {
                        auto canvas_position = to_world_space(node_editor.camera, glm::vec2{click.x, click.y});


                        if(auto clicked_text_box = get_containing_widget(node_editor, canvas_position.get()); clicked_text_box)
                        {
                            node_editor.input_state.active_text_widget = clicked_text_box->port;
                        }
                        else node_editor.input_state.active_text_widget = std::nullopt;

                        if(auto clicked_node = get_containing_node(node_editor, canvas_position.get()); clicked_node)
                        {
                            if(!click.special_key)
                                node_editor.input_state.selected_nodes.clear();

                            node_editor.input_state.selected_nodes.insert(*clicked_node);
                            move_first_focus_stack(node_editor, *clicked_node);
                        }
                        else node_editor.input_state.selected_nodes.clear();
                    },
                    [](InputEvents::Delete) {

                    },
                    [&](const InputEvents::DragBegin& drag_begin) {
                        auto canvas_position = to_world_space(node_editor.camera, glm::vec2{drag_begin.x, drag_begin.y});

                        if(auto dragged_port = get_containing_port(node_editor, canvas_position.get()); dragged_port)
                        {
                            node_editor.input_state.drag_state = ConnectionDrag {
                                    .source_port = dragged_port->port,
                                    .source_position = dragged_port->position,
                                    .destination_position = dragged_port->position
                            };

                            if(auto index = find_connection(node_editor, dragged_port->port); index)
                            {
                                Connection connection =  node_editor.graph.connections[index.value()];
                                node_editor.graph.connections.erase(node_editor.graph.connections.begin() + index.value());
                                editor_events.push_back(EditorEvents::ConnectionRemoved {
                                    connection
                                });
                            }

                            if(node_editor.compute_disabled_ports)
                            {
                                node_editor.input_state.disabled_ports = node_editor.compute_disabled_ports(dragged_port->port);
                            }
                        }
                        else if(auto dragged_node = get_containing_node(node_editor, canvas_position.get()); dragged_node)
                        {
                            if(!drag_begin.special_key)
                            {
                                node_editor.input_state.selected_nodes.clear();
                                node_editor.input_state.selected_nodes.insert(*dragged_node);
                                move_first_focus_stack(node_editor, *dragged_node);
                            }

                            node_editor.input_state.drag_state = NodeDrag {
                                    canvas_position.get()
                            };
                        }
                        else
                        {
                            if(drag_begin.special_key)
                                node_editor.input_state.drag_state = SelectDrag {
                                    {drag_begin.x, drag_begin.y},
                                    {drag_begin.x, drag_begin.y}
                                };
                            else
                                node_editor.input_state.drag_state = ViewDrag {
                                    {drag_begin.x, drag_begin.y}
                                };
                        }
                    },
                    [&](InputEvents::DragEnd) {
                        if(std::holds_alternative<SelectDrag>(node_editor.input_state.drag_state))
                        {
                            auto& select_drag = std::get<SelectDrag>(node_editor.input_state.drag_state);

                            glm::vec2 p[2] = {
                                to_world_space(node_editor.camera, select_drag.current_corner).get(),
                                to_world_space(node_editor.camera, select_drag.begin_corner).get(),
                            };

                            glm::vec2 box_size = {
                                    fabsf(p[0].x - p[1].x),
                                    fabsf(p[0].y - p[1].y)
                            };

                            glm::vec2 box_position = (p[0] + p[1]) / 2.f;


                            for(const auto&[id, node]: node_editor.graph.nodes)
                            {
                                if(contains_point(compute_bounding_box(box_position, box_size), node.position))
                                {
                                    node_editor.input_state.selected_nodes.insert(id);
                                    move_first_focus_stack(node_editor, id);
                                }
                            }
                        }

                        if(std::holds_alternative<ConnectionDrag>(node_editor.input_state.drag_state))
                        {
                            auto& connection_drag = std::get<ConnectionDrag>(node_editor.input_state.drag_state);
                            auto port = get_containing_port(node_editor, connection_drag.destination_position);

                            if(port && port->port.type != connection_drag.source_port.type && !node_editor.input_state.disabled_ports.contains(port->port))
                            {
                                if(auto index = find_connection(node_editor, port->port); index)
                                {
                                    Connection connection =  node_editor.graph.connections[index.value()];
                                    node_editor.graph.connections.erase(node_editor.graph.connections.begin() + index.value());
                                    editor_events.push_back(EditorEvents::ConnectionRemoved {
                                            connection
                                    });
                                }

                                {
                                    Connection connection;

                                    if (connection_drag.source_port.type == Port::Type::INPUT) {
                                        connection.input_port = connection_drag.source_port;
                                        connection.output_port = port->port;
                                    } else {
                                        connection.input_port = port->port;
                                        connection.output_port = connection_drag.source_port;
                                    }

                                    editor_events.push_back(EditorEvent{
                                            EditorEvents::ConnectionAdded{
                                                    connection
                                            }
                                    });

                                    node_editor.graph.connections.push_back(connection);
                                }
                            }
                            node_editor.input_state.disabled_ports.clear();
                        }

                        node_editor.input_state.drag_state = NoDrag{};
                    },
                    [&](const InputEvents::DragSustain& drag_sustain) {
                        if(std::holds_alternative<NodeDrag>(node_editor.input_state.drag_state))
                        {
                            auto& node_drag = std::get<NodeDrag>(node_editor.input_state.drag_state);

                            auto canvas_position = to_world_space(node_editor.camera, glm::vec2{drag_sustain.x, drag_sustain.y});

                            glm::vec2 delta = canvas_position - node_drag.begin_position;
                            node_drag.begin_position = canvas_position.get();

                            for(const auto& node: node_editor.input_state.selected_nodes)
                            {
                                node_editor.graph.nodes[node].position += delta;
                                compute_node(node_editor, node);
                            }
                        }

                        if(std::holds_alternative<ConnectionDrag>(node_editor.input_state.drag_state))
                        {
                            auto& connection_drag = std::get<ConnectionDrag>(node_editor.input_state.drag_state);
                            auto canvas_position = to_world_space(node_editor.camera, glm::vec2{drag_sustain.x, drag_sustain.y});

                            connection_drag.destination_position = canvas_position.get();
                        }

                        if(std::holds_alternative<SelectDrag>(node_editor.input_state.drag_state))
                        {
                            auto& select_drag = std::get<SelectDrag>(node_editor.input_state.drag_state);

                            select_drag.current_corner = {
                                drag_sustain.x,
                                drag_sustain.y
                            };
                        }

                        if(std::holds_alternative<ViewDrag>(node_editor.input_state.drag_state))
                        {
                            auto& node_drag = std::get<ViewDrag>(node_editor.input_state.drag_state);

                            glm::vec2 delta = glm::vec2{drag_sustain.x, drag_sustain.y} - node_drag.begin_position;
                            node_drag.begin_position = glm::vec2{drag_sustain.x, drag_sustain.y};

                            node_editor.camera.position -= delta;
                        }
                    },
                    [&](const InputEvents::Scroll& scroll) {
                        node_editor.camera.zoom += scroll.value;
                        node_editor.camera.zoom = glm::clamp(node_editor.camera.zoom, 0.1f, 3.f);
                    },
                    [&](const InputEvents::Character& character) {
                        if(node_editor.input_state.active_text_widget)
                        {
                            Port active_text_widget = *node_editor.input_state.active_text_widget;
                            for(auto& text_widget: node_editor.input_state.text_widget_state)
                            {
                                if(text_widget.port == active_text_widget)
                                {
                                    text_widget.state.data += character.c;
                                }
                            }
                        }
                    }
                },
                event
        );
    }
}