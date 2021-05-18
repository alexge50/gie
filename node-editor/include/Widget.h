#ifndef GIE_WIDGET_H
#define GIE_WIDGET_H

#include <Graph.h>

#include <variant>
#include <unordered_map>
#include <string>

namespace PortWidgets
{
    struct TextBox
    {
        int max_text_length = 16;
    };

    struct TextBoxState
    {
        std::string data{};
        int cursor_position = 0.f;

        CenteredBox box;
    };

    struct None
    {};
}

using PortWidget = std::variant<PortWidgets::TextBox, PortWidgets::None>;


#endif //GIE_WIDGET_H
