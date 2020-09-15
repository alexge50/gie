#include <Events.h>
#include <NodeEditor.h>
#include <detail/ComputeNode.h>

#include <algorithm>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

static bool point_in_box(glm::vec2 box_position, glm::vec2 box_size, glm::vec2 point)
{
    auto upper_left = box_position - box_size / 2.f;
    auto bottom_right = box_position + box_size / 2.f;

    return upper_left.x <= point.x && bottom_right.x >= point.x &&
           upper_left.y <= point.y && bottom_right.y >= point.y;
}

static std::optional<NodeId> get_containing_node(const Graph& graph, glm::vec2 point)
{
    auto node = std::find_if(
            graph.nodes.begin(),
            graph.nodes.end(),
            [&graph, &point](const auto& node) {
                return point_in_box(node.second.position, graph.nodes_computed.at(node.first).size, point);
            });

    if(node == graph.nodes.end())
        return std::nullopt;
    else return node->first;
}

glm::vec2 to_world_position(const NodeEditor& node_editor, glm::vec2 position)
{
    const auto view = glm::lookAt(
                glm::vec3(node_editor.camera_position, 1.f),
                glm::vec3(node_editor.camera_position, 0.f),
                glm::vec3(0.f, 1.f, 0.f));

    position -= node_editor.screen_size / 2.f;

    return glm::vec2{ (1.f / node_editor.zoom) * glm::inverse(view) * glm::vec4(position, 0.f, 1.f)};
}

void process(NodeEditor& node_editor, const std::vector<InputEvent>& input, std::vector<EditorEvent>&)
{
    for(const auto& event: input)
    {
        std::visit(
                overloaded {
                    [&](const InputEvents::Click& click) {
                        glm::vec2 canvas_position = to_world_position(node_editor, {click.x, click.y});

                        if(auto clicked_node = get_containing_node(node_editor.graph, canvas_position); clicked_node)
                        {
                            if(!click.special_key)
                                node_editor.selected_nodes.clear();

                            node_editor.selected_nodes.insert(*clicked_node);
                        }
                        else node_editor.selected_nodes.clear();
                    },
                    [](InputEvents::Delete) {

                    },
                    [&](const InputEvents::DragBegin& drag_begin) {
                        glm::vec2 canvas_position = to_world_position(node_editor, {drag_begin.x, drag_begin.y});

                        if(auto dragged_node = get_containing_node(node_editor.graph, canvas_position); dragged_node)
                        {
                            if(!drag_begin.special_key)
                            {
                                node_editor.selected_nodes.clear();
                                node_editor.selected_nodes.insert(*dragged_node);
                            }

                            node_editor.drag_state = NodeDrag {
                                    canvas_position
                            };
                        }
                        else
                        {
                            if(drag_begin.special_key)
                                node_editor.drag_state = SelectDrag {
                                    {drag_begin.x, drag_begin.y},
                                    {drag_begin.x, drag_begin.y}
                                };
                            else
                                node_editor.drag_state = ViewDrag {
                                    {drag_begin.x, drag_begin.y}
                                };
                        }
                    },
                    [&](InputEvents::DragEnd) {
                        if(std::holds_alternative<SelectDrag>(node_editor.drag_state))
                        {
                            auto& select_drag = std::get<SelectDrag>(node_editor.drag_state);

                            glm::vec2 p[2] = {
                                to_world_position(node_editor, select_drag.current_corner),
                                to_world_position(node_editor, select_drag.begin_corner),
                            };

                            glm::vec2 box_size = {
                                    fabsf(p[0].x - p[1].x),
                                    fabsf(p[0].y - p[1].y)
                            };

                            glm::vec2 box_position = (p[0] + p[1]) / 2.f;


                            for(const auto&[id, node]: node_editor.graph.nodes)
                            {
                                if(point_in_box(box_position, box_size, node.position))
                                    node_editor.selected_nodes.insert(id);
                            }
                        }

                        node_editor.drag_state = NoDrag{};
                    },
                    [&](const InputEvents::DragSustain& drag_sustain) {
                        if(std::holds_alternative<NodeDrag>(node_editor.drag_state))
                        {
                            auto& node_drag = std::get<NodeDrag>(node_editor.drag_state);

                            glm::vec2 canvas_position = to_world_position(node_editor, {drag_sustain.x, drag_sustain.y});
                            glm::vec2 delta = canvas_position - node_drag.begin_position;
                            node_drag.begin_position = canvas_position;

                            for(const auto& node: node_editor.selected_nodes)
                            {
                                node_editor.graph.nodes[node].position += delta;
                                compute_node(node_editor, node);
                            }
                        }

                        if(std::holds_alternative<SelectDrag>(node_editor.drag_state))
                        {
                            auto& select_drag = std::get<SelectDrag>(node_editor.drag_state);

                            select_drag.current_corner = {
                                drag_sustain.x,
                                drag_sustain.y
                            };
                        }

                        if(std::holds_alternative<ViewDrag>(node_editor.drag_state))
                        {
                            auto& node_drag = std::get<ViewDrag>(node_editor.drag_state);

                            glm::vec2 delta = glm::vec2{drag_sustain.x, drag_sustain.y} - node_drag.begin_position;
                            node_drag.begin_position = glm::vec2{drag_sustain.x, drag_sustain.y};

                            node_editor.camera_position -= delta;
                        }
                    },
                    [&](const InputEvents::Scroll& scroll) {
                        node_editor.zoom += scroll.value;
                        node_editor.zoom = glm::clamp(node_editor.zoom, 0.1f, 3.f);
                    }
                },
                event
        );
    }
}