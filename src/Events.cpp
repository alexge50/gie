#include <Events.h>
#include <NodeEditor.h>
#include <detail/ComputeNode.h>

#include <algorithm>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void process(NodeEditor& node_editor, const std::vector<InputEvent>& input, std::vector<EditorEvent>&)
{
    for(const auto& event: input)
    {
        std::visit(
                overloaded {
                    [](InputEvents::Click) {

                    },
                    [](InputEvents::Delete) {

                    },
                    [&](const InputEvents::DragBegin& drag_begin) {
                        auto dragged_node = std::find_if(
                                node_editor.graph.nodes.begin(),
                                node_editor.graph.nodes.end(),
                                [&node_editor, &drag_begin](const auto& node) {
                                    auto position = node.second.position;
                                    auto size = node_editor.graph.nodes_computed.at(node.first).size;

                                    auto upper_left = position - size / 2.f;
                                    auto bottom_right = position + size / 2.f;

                                    return upper_left.x <= drag_begin.x && bottom_right.x >= drag_begin.x &&
                                           upper_left.y <= drag_begin.y && bottom_right.y >= drag_begin.y;
                                });

                        if(dragged_node != node_editor.graph.nodes.end())
                        {
                            node_editor.drag_state = NodeDrag {
                                dragged_node->first,
                                {drag_begin.x, drag_begin.y}
                            };
                        }
                    },
                    [&](InputEvents::DragEnd) {
                        node_editor.drag_state = NoDrag{};
                    },
                    [&](const InputEvents::DragSustain& drag_sustain) {
                        if(std::holds_alternative<NodeDrag>(node_editor.drag_state))
                        {
                            NodeDrag& node_drag = std::get<NodeDrag>(node_editor.drag_state);

                            glm::vec2 delta = glm::vec2{drag_sustain.x, drag_sustain.y} - node_drag.begin_position;
                            node_drag.begin_position = glm::vec2{drag_sustain.x, drag_sustain.y};

                            node_editor.graph.nodes[node_drag.node].position += delta;
                            compute_node(node_editor, node_drag.node);
                        }
                    },
                    [](InputEvents::Scroll) {

                    }
                },
                event
        );
    }
}