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

    struct Scroll
    {
        float value;
    };
}

namespace EditorEvents
{
    struct ConnectionAdded
    {
        Connection connection;
    };

    struct ConnectionRemoved
    {

    };

    struct NodeRemoved
    {

    };
}

using InputEvent = std::variant<InputEvents::Click, InputEvents::Delete, InputEvents::DragBegin, InputEvents::DragEnd, InputEvents::DragSustain, InputEvents::Scroll>;
using EditorEvent = std::variant<EditorEvents::ConnectionAdded, EditorEvents::ConnectionRemoved, EditorEvents::NodeRemoved>;

struct NodeEditor;
void process(NodeEditor&, const std::vector<InputEvent>&, std::vector<EditorEvent>&);

#endif //NODE_EDITOR_EVENTS_H
