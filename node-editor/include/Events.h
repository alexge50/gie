#ifndef NODE_EDITOR_EVENTS_H
#define NODE_EDITOR_EVENTS_H

#include <vector>
#include <variant>

#include <Graph.h>

namespace InputEvents
{
    struct DragBegin
    {
        float x, y;
        bool special_key; // for drag select
    };

    struct DragSustain
    {
        float x, y;
    };

    struct DragEnd {};

    struct Click
    {
        float x, y;
        bool special_key;
    };

    struct Delete {};

    struct Character
    {
        char c;
    };

    struct Scroll
    {
        float value;
    };
}

using InputEvent = std::variant<InputEvents::Click, InputEvents::Delete, InputEvents::DragBegin, InputEvents::DragEnd, InputEvents::DragSustain, InputEvents::Scroll, InputEvents::Character>;


namespace EditorEvents
{
    struct ConnectionAdded
    {
        Connection connection;
    };

    struct ConnectionRemoved
    {
        Connection connection;
    };

    struct NodeRemoved
    {
        NodeId node_id;
    };

    struct SelectedNodesMoved
    {
        glm::vec2 delta;
    };

    struct ConnectionDrag
    {
        Port source_port;
    };

    struct ConnectionDragEnded
    {

    };

    struct WidgetInputEvent
    {
        Port widget_port;
        InputEvent input_event;
    };
}

using EditorEvent = std::variant<EditorEvents::ConnectionAdded, EditorEvents::ConnectionRemoved, EditorEvents::NodeRemoved, EditorEvents::WidgetInputEvent, EditorEvents::ConnectionDrag, EditorEvents::SelectedNodesMoved, EditorEvents::ConnectionDragEnded>;

struct NodeEditor;
void process(NodeEditor&, const std::vector<InputEvent>&, std::vector<EditorEvent>&);

#endif //NODE_EDITOR_EVENTS_H
