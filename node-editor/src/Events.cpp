#include <Events.h>
#include <NodeEditor.h>

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

static std::optional<InteractiveElementState> get_interactive_element(const NodeEditorState& state, glm::vec2 point)
{
    for(const auto& interactive_element: state.interactive_element_state)
    {
        if(std::visit([&](const auto& area) -> bool {
            return contains_point(area, point);
        }, interactive_element.area))
            return interactive_element;
    }

    return std::nullopt;
}

static void clear_selected(NodeEditorState& state)
{
    for(auto& [node_id, node]: state.node_state)
    {
        node.selected = false;
    }
}

static void move_first_focus_stack(NodeEditorState& state, NodeId node)
{
    std::erase(state.focus_stack, node);
    state.focus_stack.insert(state.focus_stack.begin(), node);
}


static void process(NodeEditorState& state, const std::vector<InputEvent>& input, std::vector<EditorEvent>& output)
{
    for(const auto& event: input)
    {
        std::visit(
                overloaded {
                        [&](const InputEvents::Click& click) {
                            if(auto element = get_interactive_element(state, click.position.get()))
                            {
                                if(state.input_state.active_text_widget)
                                    state.widget_metadata[*state.input_state.active_text_widget].active = false;

                                if(auto node = std::get_if<InteractiveElementState::Node>(&element->element))
                                {
                                    if(!click.special_key)
                                        clear_selected(state);

                                    move_first_focus_stack(state, node->node_id);
                                    state.node_state[node->node_id].selected = true;
                                }
                                else if(auto widget = std::get_if<InteractiveElementState::Widget>(&element->element))
                                {
                                    state.input_state.active_text_widget = widget->port_id;
                                    state.widget_metadata[widget->port_id].active = true;
                                }
                            }
                        },
                        [](InputEvents::Delete) {

                        },
                        [&](const InputEvents::DragBegin& drag_begin) {
                            if(state.input_state.active_text_widget && contains_point(state.widget_metadata[*state.input_state.active_text_widget].box, drag_begin.position.get()))
                            {
                                output.emplace_back(
                                        EditorEvents::WidgetInputEvent{
                                                .widget_port = *state.input_state.active_text_widget,
                                                .input_event = drag_begin
                                        }
                                );

                                return ;
                            }
                            else
                            {
                                state.input_state.active_text_widget = std::nullopt;
                            }

                            if(auto element = get_interactive_element(state, drag_begin.position.get()))
                            {
                                if(auto node = std::get_if<InteractiveElementState::Node>(&element->element))
                                {
                                    if(!drag_begin.special_key)
                                    {
                                        clear_selected(state);
                                        move_first_focus_stack(state, node->node_id);
                                        state.node_state[node->node_id].selected = true;
                                    }

                                    state.input_state.drag_state = NodeDrag {
                                            drag_begin.position.get()
                                    };
                                }
                                else if(auto port = std::get_if<InteractiveElementState::Port>(&element->element))
                                {
                                    state.input_state.drag_state = ConnectionDrag {
                                            .source_port = port->port_id,
                                            .source_position = drag_begin.position.get(),
                                            .destination_position = drag_begin.position.get()
                                    };

                                    output.emplace_back(EditorEvents::ConnectionDrag{
                                        port->port_id
                                    });
                                }
                            }
                            else
                            {
                                if(drag_begin.special_key)
                                {
                                    state.input_state.drag_state = SelectDrag {
                                            drag_begin.position.get(),
                                            drag_begin.position.get()
                                    };
                                }
                                else
                                {
                                    state.input_state.drag_state = ViewDrag {
                                            drag_begin.screen_space_position.get()
                                    };
                                }
                            }
                        },
                        [&](InputEvents::DragEnd) {
                            if(state.input_state.active_text_widget)
                            {
                                output.emplace_back(
                                        EditorEvents::WidgetInputEvent{
                                                .widget_port = *state.input_state.active_text_widget,
                                                .input_event = InputEvents::DragEnd{}
                                        }
                                );

                                return ;
                            }

                            if(std::holds_alternative<SelectDrag>(state.input_state.drag_state))
                            {
                                auto& select_drag = std::get<SelectDrag>(state.input_state.drag_state);

                                glm::vec2 p[2] = {
                                        select_drag.current_corner,
                                        select_drag.begin_corner,
                                };

                                glm::vec2 box_size = {
                                        fabsf(p[0].x - p[1].x),
                                        fabsf(p[0].y - p[1].y)
                                };

                                glm::vec2 box_position = (p[0] + p[1]) / 2.f;

                                for(const auto&[id, node]: state.node_state)
                                {
                                    if(contains_point(compute_bounding_box(box_position, box_size), node.position))
                                    {
                                        state.node_state[id].selected = true;
                                        move_first_focus_stack(state, id);
                                    }
                                }
                            }

                            if(std::holds_alternative<ConnectionDrag>(state.input_state.drag_state))
                            {
                                auto& connection_drag = std::get<ConnectionDrag>(state.input_state.drag_state);
                                auto port_ptr = get_interactive_element(state, connection_drag.destination_position);

                                if(port_ptr)
                                {
                                    if(auto port = std::get_if<InteractiveElementState::Port>(&port_ptr->element))
                                    {
                                        Connection connection;

                                        if (connection_drag.source_port.type == Port::Type::INPUT) {
                                            connection.input_port = connection_drag.source_port;
                                            connection.output_port = port->port_id;
                                        } else {
                                            connection.input_port = port->port_id;
                                            connection.output_port = connection_drag.source_port;
                                        }

                                        output.emplace_back(
                                                EditorEvents::ConnectionAdded{
                                                        connection
                                                }
                                        );
                                    }
                                }
                                output.emplace_back(EditorEvents::ConnectionDragEnded{});
                            }

                            state.input_state.drag_state = NoDrag{};
                        },
                        [&](const InputEvents::DragSustain& drag_sustain) {
                            if(state.input_state.active_text_widget && contains_point(state.widget_metadata[*state.input_state.active_text_widget].box, drag_sustain.position.get()))
                            {
                                output.emplace_back(
                                        EditorEvents::WidgetInputEvent{
                                                .widget_port = *state.input_state.active_text_widget,
                                                .input_event = drag_sustain
                                        }
                                );

                                return ;
                            }

                            if(std::holds_alternative<NodeDrag>(state.input_state.drag_state))
                            {
                                auto& node_drag = std::get<NodeDrag>(state.input_state.drag_state);

                                glm::vec2 delta = drag_sustain.position.get() - node_drag.begin_position;
                                node_drag.begin_position = drag_sustain.position.get();

                                output.emplace_back(
                                        EditorEvents::SelectedNodesMoved{
                                                delta,
                                        }
                                );
                            }

                            if(std::holds_alternative<ConnectionDrag>(state.input_state.drag_state))
                            {
                                auto& connection_drag = std::get<ConnectionDrag>(state.input_state.drag_state);

                                connection_drag.destination_position = drag_sustain.position.get();
                            }

                            if(std::holds_alternative<SelectDrag>(state.input_state.drag_state))
                            {
                                auto& select_drag = std::get<SelectDrag>(state.input_state.drag_state);

                                select_drag.current_corner = drag_sustain.position.get();
                            }

                            if(std::holds_alternative<ViewDrag>(state.input_state.drag_state))
                            {
                                auto& view_drag = std::get<ViewDrag>(state.input_state.drag_state);

                                glm::vec2 delta = drag_sustain.screen_space_position.get() - view_drag.begin_position;
                                view_drag.begin_position = drag_sustain.screen_space_position.get();

                                output.emplace_back(EditorEvents::CameraModified{
                                   .delta_position = -delta,
                                   .delta_zoom = 0.f
                                });
                            }
                        },
                        [&](const InputEvents::Scroll& scroll) {
                            if(state.input_state.active_text_widget)
                            {
                                output.emplace_back(
                                        EditorEvents::WidgetInputEvent{
                                                .widget_port = *state.input_state.active_text_widget,
                                                .input_event = scroll
                                        }
                                );

                                return ;
                            }

                            output.emplace_back(EditorEvents::CameraModified{
                                    .delta_position = glm::vec2{0.f, 0.f},
                                    .delta_zoom = scroll.value
                            });
                        },
                        [&](const InputEvents::Character& character) {
                            if(state.input_state.active_text_widget)
                            {
                                output.emplace_back(
                                        EditorEvents::WidgetInputEvent{
                                                .widget_port = *state.input_state.active_text_widget,
                                                .input_event = character
                                        }
                                );
                            }

                        }
                },
                event
        );
    }
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

void process(NodeEditor& node_editor, const std::vector<InputEvent>& input, std::vector<EditorEvent>& output)
{
    process(node_editor.state, input, output);

    for(size_t i = 0; i < output.size(); i++)
    {
        const auto& event = output[i];

        if(auto node_moved = std::get_if<EditorEvents::SelectedNodesMoved>(&event))
        {
            for(const auto& [node_id, node]: node_editor.state.node_state)
            {
                if(node.selected)
                {
                    node_editor.graph.nodes.at(node_id).position += node_moved->delta;
                }
            }
        }
        else if(auto widget_input = std::get_if<EditorEvents::WidgetInputEvent>(&event))
        {
            if(auto character = std::get_if<InputEvents::Character>(&widget_input->input_event))
            {
                Port active_text_widget = *node_editor.state.input_state.active_text_widget;
                node_editor.state.text_box_state[active_text_widget].data += character->c;
            }
        }
        else if(auto connection_drag = std::get_if<EditorEvents::ConnectionDrag>(&event))
        {
            for(auto& [port_id, port]: node_editor.state.port_state)
            {
                if(port_id.type == connection_drag->source_port.type && port_id != connection_drag->source_port)
                {
                    port.disabled = true;
                }
            }
        }
        else if(std::get_if<EditorEvents::ConnectionDragEnded>(&event))
        {
            for(auto& [port_id, port]: node_editor.state.port_state)
            {
                port.disabled = false;
            }
        }
        else if(auto connection_added = std::get_if<EditorEvents::ConnectionAdded>(&event))
        {
            if(auto connection = find_connection(node_editor, connection_added->connection.input_port))
            {
                size_t connection_index = *connection;

                output.emplace_back(EditorEvents::ConnectionRemoved{
                    .connection = node_editor.graph.connections[connection_index]
                });
                node_editor.graph.connections.erase(node_editor.graph.connections.begin() + connection_index);
            }
            node_editor.graph.connections.emplace_back(connection_added->connection);
        }
        else if(auto camera_modified = std::get_if<EditorEvents::CameraModified>(&event))
        {
            node_editor.camera.position += camera_modified->delta_position;
            node_editor.camera.zoom += camera_modified->delta_zoom;
            node_editor.camera.zoom = glm::clamp(node_editor.camera.zoom, 0.1f, 3.f);
        }
    }
}