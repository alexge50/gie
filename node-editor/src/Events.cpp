#include <Events.h>
#include <NodeEditor.h>

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>

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


static void process(NodeEditorState& state, const InputEventVector& input, EditorEventVector& output)
{
    for(const auto& click: filter_events<InputEvents::Click>(input))
    {
        bool widget_still_active = false;

        if(auto element = get_interactive_element(state, click.position.get()))
        {
            if(auto node = std::get_if<InteractiveElementState::Node>(&element->element))
            {
                if(!click.special_key)
                    clear_selected(state);

                move_first_focus_stack(state, node->node_id);
                state.node_state[node->node_id].selected = true;
            }
            else if(auto widget = std::get_if<InteractiveElementState::Widget>(&element->element))
            {
                if(!state.input_state.active_widget)
                {
                    output.add(EditorEvents::WidgetActivated {
                            widget->widget_id
                    });

                    output.add(EditorEvents::WidgetInputEvent {
                       .widget = widget->widget_id,
                       .input_event = click
                    });

                    state.input_state.active_widget = widget->widget_id;
                    state.widget_state[*state.input_state.active_widget].active = true;
                    widget_still_active = true;
                }
                else if(widget->widget_id != *state.input_state.active_widget)
                {
                    output.add(EditorEvents::WidgetDeactivated {
                        *state.input_state.active_widget
                    });

                    output.add(EditorEvents::WidgetActivated {
                            widget->widget_id
                    });

                    output.add(EditorEvents::WidgetInputEvent {
                            .widget = widget->widget_id,
                            .input_event = click
                    });

                    state.widget_state[*state.input_state.active_widget].active = false;
                    state.input_state.active_widget = std::nullopt;

                    state.input_state.active_widget = widget->widget_id;
                    state.widget_state[*state.input_state.active_widget].active = true;
                    widget_still_active = true;
                }
                else widget_still_active = true;
            }
        }

        if(!widget_still_active)
        {
            if(state.input_state.active_widget)
            {
                output.add(EditorEvents::WidgetDeactivated {
                        *state.input_state.active_widget
                });
                state.widget_state[*state.input_state.active_widget].active = false;
                state.input_state.active_widget = std::nullopt;
            }
        }
        else
        {
            output.add(EditorEvents::WidgetInputEvent{
                    .widget = *state.input_state.active_widget,
                    .input_event = click
            });
        }
    }

    for(const auto& key_pressed: filter_events<InputEvents::KeyPressed>(input))
    {
        if(state.input_state.active_widget)
        {
            output.add(
                    EditorEvents::WidgetInputEvent{
                            .widget = *state.input_state.active_widget,
                            .input_event = key_pressed
                    }
            );

            return ;
        }
    }

    for(const auto& drag_begin: filter_events<InputEvents::DragBegin>(input))
    {
        if(auto element = get_interactive_element(state, drag_begin.position.get()))
        {
            if(auto node = std::get_if<InteractiveElementState::Node>(&element->element))
            {
                state.input_state.active_widget = std::nullopt;

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
                state.input_state.active_widget = std::nullopt;

                state.input_state.drag_state = ConnectionDrag {
                        .source_port = port->port_id,
                        .source_position = drag_begin.position.get(),
                        .destination_position = drag_begin.position.get()
                };

                output.add(EditorEvents::ConnectionDrag{
                        port->port_id
                });
            }
            else if(auto widget = std::get_if<InteractiveElementState::Widget>(&element->element))
            {
                if(state.input_state.active_widget && *state.input_state.active_widget != widget->widget_id)
                {
                    output.add(EditorEvents::WidgetDeactivated {
                            *state.input_state.active_widget
                    });

                    output.add(EditorEvents::WidgetActivated {
                            widget->widget_id
                    });

                    state.widget_state[*state.input_state.active_widget].active = false;
                    state.input_state.active_widget = widget->widget_id;
                    state.widget_state[*state.input_state.active_widget].active = true;
                }
                else if(!state.input_state.active_widget)
                {
                    output.add(EditorEvents::WidgetActivated {
                            widget->widget_id
                    });

                    state.input_state.active_widget = widget->widget_id;
                    state.widget_state[*state.input_state.active_widget].active = true;
                }

                output.add(EditorEvents::WidgetInputEvent{
                    .widget = *state.input_state.active_widget,
                    .input_event = drag_begin
                });
            }
        }
        else
        {
            if(state.input_state.active_widget)
            {
                output.add(EditorEvents::WidgetDeactivated {
                        *state.input_state.active_widget
                });
                state.widget_state[*state.input_state.active_widget].active = false;
                state.input_state.active_widget = std::nullopt;
            }

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
    }

    for([[maybe_unused]] const auto& drag_end: filter_events<InputEvents::DragEnd>(input))
    {
        if(state.input_state.active_widget)
        {
            output.add(
                    EditorEvents::WidgetInputEvent{
                            .widget = *state.input_state.active_widget,
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

                    output.add(
                            EditorEvents::ConnectionAdded{
                                    connection
                            }
                    );
                }
            }
            output.add(EditorEvents::ConnectionDragEnded{});
        }

        state.input_state.drag_state = NoDrag{};
    }

    for(const auto& drag_sustain: filter_events<InputEvents::DragSustain>(input))
    {
        if(state.input_state.active_widget)
        {
            output.add(
                    EditorEvents::WidgetInputEvent{
                            .widget = *state.input_state.active_widget,
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

            output.add(
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

            output.add(EditorEvents::CameraModified{
                    .delta_position = -delta,
                    .delta_zoom = 0.f
            });
        }
    }

    for(const auto& character: filter_events<InputEvents::Character>(input))
    {
        if(state.input_state.active_widget)
        {
            output.add(
                    EditorEvents::WidgetInputEvent{
                            .widget = *state.input_state.active_widget,
                            .input_event = character
                    }
            );

            return ;
        }
    }

    for(const auto& key: filter_events<InputEvents::KeyPressed>(input))
    {
        if(state.input_state.active_widget)
        {
            output.add(
                    EditorEvents::WidgetInputEvent{
                            .widget = *state.input_state.active_widget,
                            .input_event = key
                    }
            );

            return ;
        }
    }

    for(const auto& scroll: filter_events<InputEvents::Scroll>(input))
    {
        if(state.input_state.active_widget)
        {
            output.add(
                    EditorEvents::WidgetInputEvent{
                            .widget = *state.input_state.active_widget,
                            .input_event = scroll
                    }
            );

            return ;
        }

        output.add(EditorEvents::CameraModified{
                .delta_position = glm::vec2{0.f, 0.f},
                .delta_zoom = scroll.value
        });
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

void process_color_picker(Widgets::ColorPickerState& state, const InputEvent& input_event)
{
    auto process_wheel = [&](glm::vec2 mouse_position)
    {
        auto luminance = glm::hsvColor(state.color).z;

        glm::vec2 position = mouse_position - state.popup->color_wheel.center;
        position /= state.popup->color_wheel.size.x / 2.f;
        position = glm::clamp(position, -1.f, 1.f);

        float theta = glm::atan(position.y, position.x);
        float offset = glm::sqrt(position.x * position.x + position.y * position.y);

        theta += M_PI;
        theta /= 2 * M_PI;

        state.color = glm::rgbColor(glm::vec3(theta * 360.f, offset, luminance));
    };

    auto process_luminance = [&](glm::vec2 mouse_position)
    {
        auto hsv_color = glm::hsvColor(state.color);

        float luminance = 0.5f - (mouse_position - state.popup->luminance_bar.center).y
                                 / state.popup->luminance_bar.size.y;
        luminance = glm::clamp(luminance, 0.f, 1.f);
        hsv_color.z = luminance;
        state.color = glm::rgbColor(hsv_color);
    };

    if(auto click = std::get_if<InputEvents::Click>(&input_event))
    {
        if(contains_point(Circle{
                state.popup->color_wheel.center,
                state.popup->color_wheel.size.x / 2.f
        }, click->position.get()))
        {
            process_wheel(click->position.get());
        }

        if(contains_point(state.popup->luminance_bar, click->position.get()))
        {
            process_luminance(click->position.get());
        }
    }
    else if(auto drag_begin = std::get_if<InputEvents::DragBegin>(&input_event))
    {
        if(contains_point(Circle{
                state.popup->color_wheel.center,
                state.popup->color_wheel.size.x / 2.f
        }, drag_begin->position.get()))
        {
            state.popup->drag_wheel = true;
            process_wheel(drag_begin->position.get());
        }

        if(contains_point(state.popup->luminance_bar, drag_begin->position.get()))
        {
            state.popup->drag_luminance_bar = true;
            process_luminance(drag_begin->position.get());
        }
    }
    else if(auto drag_sustain = std::get_if<InputEvents::DragSustain>(&input_event))
    {
        if(state.popup->drag_wheel)
        {
            process_wheel(drag_sustain->position.get());
        }

        if(state.popup->drag_luminance_bar)
        {
            process_luminance(drag_sustain->position.get());
        }
    }
    else if(std::get_if<InputEvents::DragEnd>(&input_event))
    {
        state.popup->drag_wheel = false;
        state.popup->drag_luminance_bar = false;
    }
}

void process(NodeEditor& node_editor, const InputEventVector& input, EditorEventVector& output)
{
    process(node_editor.state, input, output);

    for(auto node_moved: filter_events<EditorEvents::SelectedNodesMoved>(output))
    {
        for(const auto& [node_id, node]: node_editor.state.node_state)
        {
            if(node.selected)
            {
                node_editor.graph.nodes.at(node_id).position += node_moved->delta;
            }
        }
    }

    for(auto widget_input: filter_events<EditorEvents::WidgetInputEvent>(output))
    {
        WidgetId active_widget = widget_input->widget;

        if(auto* text_widget = std::get_if<Widgets::TextBoxState>(&node_editor.state.widget_state[active_widget].state))
        {
            if(auto character = std::get_if<InputEvents::Character>(&widget_input->input_event))
            {
                text_widget->data.insert(text_widget->data.begin() + text_widget->cursor_position, character->c);
                text_widget->cursor_position += 1;
            }
            else if(auto key = std::get_if<InputEvents::KeyPressed>(&widget_input->input_event))
            {
                if(key->key == InputEvents::KeyPressed::Key::RIGHT_ARROW)
                {
                    text_widget->cursor_position = std::min(text_widget->cursor_position + 1, text_widget->data.size());
                }
                else if(key->key == InputEvents::KeyPressed::Key::LEFT_ARROW)
                {
                    if(text_widget->cursor_position != 0)
                    {
                        text_widget->cursor_position -= 1;
                    }
                }
                else if(key->key == InputEvents::KeyPressed::Key::BACKSPACE)
                {
                    if(text_widget->cursor_position != 0)
                    {
                        text_widget->data.erase(text_widget->data.begin() + (text_widget->cursor_position - 1));
                        text_widget->cursor_position -= 1;
                    }
                }
                else if(key->key == InputEvents::KeyPressed::Key::DELETE)
                {
                    if(text_widget->cursor_position != text_widget->data.size())
                    {
                        text_widget->data.erase(text_widget->data.begin() + (text_widget->cursor_position));
                    }
                }
                else if(key->key == InputEvents::KeyPressed::Key::ENTER)
                {
                    node_editor.state.widget_state[active_widget].active = false;
                    node_editor.state.input_state.active_widget = std::nullopt;

                    output.add(EditorEvents::WidgetDeactivated {
                        active_widget
                    });
                }
            }

            float cursor_position = node_editor.font->compute_bounding_box(
                    std::string_view{text_widget->data.data(), std::min(text_widget->data.size(), text_widget->cursor_position)},
                    node_editor.styling_config.text_height
            ).x;

            if(cursor_position - text_widget->view_position < 0.f)
            {
                text_widget->view_position = cursor_position;
            }
            else if(cursor_position - text_widget->view_position > text_widget->text_box.size.x)
            {
                text_widget->view_position = cursor_position - text_widget->text_box.size.x;
            }
        }
        else if(auto color_picker = std::get_if<Widgets::ColorPickerState>(&node_editor.state.widget_state[active_widget].state))
        {
            process_color_picker(*color_picker, widget_input->input_event);
        }
    }

    for(auto widget_deactivated: filter_events<EditorEvents::WidgetDeactivated>(output))
    {
        output.add(EditorEvents::WidgetValueChanged{
            widget_deactivated->widget
        });

        if([[maybe_unused]]auto* color_picker = std::get_if<Widgets::ColorPickerState>(&node_editor.state.widget_state[widget_deactivated->widget].state))
        {
            node_editor.state.widget_state[widget_deactivated->widget].popup = false;
        }
    }

    for(auto widget_activated: filter_events<EditorEvents::WidgetActivated>(output))
    {
        if([[maybe_unused]]auto* color_picker = std::get_if<Widgets::ColorPickerState>(&node_editor.state.widget_state[widget_activated->widget].state))
        {
            node_editor.state.widget_state[widget_activated->widget].popup = true;
        }
    }

    for(auto connection_drag: filter_events<EditorEvents::ConnectionDrag>(output))
    {
        for(auto& [port_id, port]: node_editor.state.port_state)
        {
            if(port_id.type == connection_drag->source_port.type && port_id != connection_drag->source_port)
            {
                port.disabled = true;
            }
        }
    }

    for([[maybe_unused]] auto _: filter_events<EditorEvents::ConnectionDragEnded>(output))
    {
        for(auto& [port_id, port]: node_editor.state.port_state)
        {
            port.disabled = false;
        }
    }

    for(auto connection_added: filter_events<EditorEvents::ConnectionAdded>(output))
    {
        if(auto connection = find_connection(node_editor, connection_added->connection.input_port))
        {
            size_t connection_index = *connection;

            output.add(EditorEvents::ConnectionRemoved{
                    .connection = node_editor.graph.connections[connection_index]
            });
            node_editor.graph.connections.erase(node_editor.graph.connections.begin() + connection_index);
        }
        node_editor.graph.connections.emplace_back(connection_added->connection);
    }

    for(auto camera_modified: filter_events<EditorEvents::CameraModified>(output))
    {
        node_editor.camera.position += camera_modified->delta_position;
        node_editor.camera.zoom += camera_modified->delta_zoom;
        node_editor.camera.zoom = glm::clamp(node_editor.camera.zoom, 0.1f, 3.f);

    }
}