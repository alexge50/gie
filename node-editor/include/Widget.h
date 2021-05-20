#ifndef GIE_WIDGET_H
#define GIE_WIDGET_H

#include <Graph.h>

#include <variant>
#include <unordered_map>
#include <string>

namespace Widgets
{
    struct TextBox
    {
        int max_text_length = 16;
    };

    struct TextBoxState
    {
        std::string data{};
        int cursor_position = 0;
    };

    struct None
    {};
}

using Widget = std::variant<Widgets::TextBox, Widgets::None>;
using WidgetState = std::variant<Widgets::TextBoxState, Widgets::None>;



#endif //GIE_WIDGET_H
