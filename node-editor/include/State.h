#ifndef GIE_STATE_H
#define GIE_STATE_H

#include <Graph.h>
#include <InputState.h>
#include <Camera.h>
#include <WidgetId.h>

#include <glm/vec2.hpp>
#include <detail/BoundingArea.h>

#include <variant>
#include <vector>
#include <unordered_map>

struct NodeTypeState
{
    glm::vec2 size;
    glm::vec3 color;
    glm::vec2 header_position;
    glm::vec2 header_size;
    std::string title;
    int input_port_count;
    int output_port_count;
};

struct NodeTypeInputPortState
{
    glm::vec3 color;
    glm::vec2 position;
    std::string label;
    float radius;
    CenteredBox widget_box;
    Widget port_widget;
};

struct NodeTypeOutputPortState
{
    glm::vec3 color;
    glm::vec2 position;
    std::string label;
    float radius;
};

struct NodeState
{
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 header_position;
    glm::vec2 header_size;
    glm::vec3 color;
    std::string_view title;
    int order;
    bool selected = false;
};

struct PortState
{
    glm::vec2 position;
    glm::vec3 color;
    std::string_view label;
    float radius;
    int order;
    bool disabled = false;
};

struct InteractiveElementState
{
    struct Port
    {
        ::Port port_id;
    };

    struct Node
    {
        NodeId node_id;
    };

    struct Widget
    {
        WidgetId widget_id;
    };

    std::variant<Port, Node, Widget> element;
    std::variant<CenteredBox, Circle> area;
    int order;
};

struct WidgetDataState
{
    WidgetState state = Widgets::None{};
    CenteredBox box;
    int order;
    bool active;
};

struct NodeEditorState
{
    // node type "state"
    std::unordered_map<NodeTypeId, NodeTypeState> node_type_state;
    std::unordered_map<Port, NodeTypeInputPortState, PortHasher> node_type_input_port_state; // node_id of Port is node_type_id
    std::unordered_map<Port, NodeTypeOutputPortState, PortHasher> node_type_output_port_state; // node_id of Port is node_type_id
    std::unordered_map<NodeId, NodeState> node_state;
    std::unordered_map<Port, PortState, PortHasher> port_state;
    std::vector<InteractiveElementState> interactive_element_state;
    std::unordered_map<WidgetId, WidgetDataState, WidgetIdHasher> widget_state;
    std::vector<NodeId> focus_stack;
    std::unordered_map<NodeId, int> order;

    InputState input_state;
};

struct NodeEditor;
void compute_state(const NodeEditor&, NodeEditorState&);

#endif //GIE_STATE_H
