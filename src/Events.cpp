#include <Events.h>
#include <NodeEditor.h>
#include <detail/ComputeNode.h>

#include <algorithm>
#include <math.h>

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


void process(NodeEditor& node_editor, const std::vector<InputEvent>& input, std::vector<EditorEvent>&)
{
    for(const auto& event: input)
    {
        std::visit(
                overloaded {
                    [&](const InputEvents::Click& click) {
                        if(auto clicked_node = get_containing_node(node_editor.graph, {click.x, click.y}); clicked_node)
                            node_editor.selected_nodes.insert(*clicked_node);
                        else node_editor.selected_nodes.clear();
                    },
                    [](InputEvents::Delete) {

                    },
                    [&](const InputEvents::DragBegin& drag_begin) {

                        if(auto dragged_node = get_containing_node(node_editor.graph, {drag_begin.x, drag_begin.y}); dragged_node)
                        {
                            node_editor.drag_state = NodeDrag {
                                *dragged_node,
                                {drag_begin.x, drag_begin.y}
                            };
                        }
                        else
                        {
                            if(drag_begin.special_key)
                                node_editor.drag_state = SelectDrag {
                                    {drag_begin.x, drag_begin.y},
                                    {drag_begin.x, drag_begin.y}
                                };
                        }
                    },
                    [&](InputEvents::DragEnd) {
                        if(std::holds_alternative<SelectDrag>(node_editor.drag_state))
                        {
                            auto& select_drag = std::get<SelectDrag>(node_editor.drag_state);

                            glm::vec2 box_size = {
                                    fabsf(select_drag.current_corner.x - select_drag.begin_corner.x),
                                    fabsf(select_drag.current_corner.y - select_drag.begin_corner.y)
                            };

                            glm::vec2 box_position = (select_drag.current_corner + select_drag.begin_corner) / 2.f;


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

                            glm::vec2 delta = glm::vec2{drag_sustain.x, drag_sustain.y} - node_drag.begin_position;
                            node_drag.begin_position = glm::vec2{drag_sustain.x, drag_sustain.y};

                            node_editor.graph.nodes[node_drag.node].position += delta;
                            compute_node(node_editor, node_drag.node);
                        }

                        if(std::holds_alternative<SelectDrag>(node_editor.drag_state))
                        {
                            auto& select_drag = std::get<SelectDrag>(node_editor.drag_state);

                            select_drag.current_corner = {
                                drag_sustain.x,
                                drag_sustain.y
                            };
                        }
                    },
                    [](InputEvents::Scroll) {

                    }
                },
                event
        );
    }
}