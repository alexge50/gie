#ifndef NODE_EDITOR_EVENTS_H
#define NODE_EDITOR_EVENTS_H

#include <vector>
#include <variant>

#include <Graph.h>
#include <Camera.h>
#include <WidgetId.h>

#include <detail/EventVector.h>

namespace InputEvents
{
    struct DragBegin
    {
        WorldSpaceCoordinates position;
        ScreenSpaceCoordinates screen_space_position;
        bool special_key; // for drag select
    };

    struct DragSustain
    {
        WorldSpaceCoordinates position;
        ScreenSpaceCoordinates screen_space_position;
    };

    struct DragEnd {};

    struct Click
    {
        WorldSpaceCoordinates position;
        bool special_key;
    };

    struct KeyPressed
    {
        enum class Key: char
        {
            DELETE = 0,
            LEFT_ARROW = 1,
            RIGHT_ARROW = 2,
            UP_ARROW = 3,
            DOWN_ARROW = 4,
            BACKSPACE = 5,
            ENTER = 6
        };

        Key key;
    };

    struct Character
    {
        char c;
    };

    struct Scroll
    {
        float value;
    };
}

using InputEventVector = EventVector<InputEvents::Click, InputEvents::KeyPressed, InputEvents::DragBegin, InputEvents::DragEnd, InputEvents::DragSustain, InputEvents::Scroll, InputEvents::Character>;
using InputEvent = std::variant<InputEvents::Click, InputEvents::KeyPressed, InputEvents::DragBegin, InputEvents::DragEnd, InputEvents::DragSustain, InputEvents::Scroll, InputEvents::Character>;

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
        WidgetId widget;
        InputEvent input_event;
    };

    struct WidgetDeactivated
    {
        WidgetId widget;
    };

    struct WidgetValueChanged
    {
        WidgetId widget;
    };

    struct CameraModified
    {
        glm::vec2 delta_position;
        float delta_zoom;
    };
}

using EditorEventVector = EventVector<EditorEvents::ConnectionAdded, EditorEvents::ConnectionRemoved, EditorEvents::NodeRemoved, EditorEvents::WidgetInputEvent, EditorEvents::ConnectionDrag, EditorEvents::SelectedNodesMoved, EditorEvents::ConnectionDragEnded, EditorEvents::CameraModified, EditorEvents::WidgetDeactivated, EditorEvents::WidgetValueChanged>;

struct NodeEditor;
void process(NodeEditor&, const InputEventVector &, EditorEventVector &);

#endif //NODE_EDITOR_EVENTS_H
