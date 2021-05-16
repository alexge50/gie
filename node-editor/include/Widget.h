#ifndef GIE_WIDGET_H
#define GIE_WIDGET_H

#include <variant>

namespace PortWidgets
{
    struct TextBox
    {
        int max_text_length = 16;
    };

    struct None
    {};
}

using PortWidget = std::variant<PortWidgets::TextBox, PortWidgets::None>;


#endif //GIE_WIDGET_H
