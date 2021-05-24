#ifndef GIE_WIDGET_H
#define GIE_WIDGET_H

#include <Graph.h>
#include <detail/BoundingArea.h>

#include <variant>
#include <unordered_map>
#include <string>
#include <optional>

#include <glm/vec3.hpp>

namespace Widgets
{
    struct TextBox
    {
        int max_text_length = 16;
    };

    struct TextBoxState
    {
        std::string data{};
        size_t cursor_position = 0;
        float view_position = 0.f;

        CenteredBox text_box;
    };

    struct ColorPicker
    {};

    struct ColorPickerState
    {
        struct Popup
        {
            CenteredBox box;
            CenteredBox color_wheel;
            CenteredBox luminance_bar;
            bool drag_wheel = false;
            bool drag_luminance_bar = false;

            TextBoxState channel_text_box[3];
            CenteredBox channel_input_area[3];
            std::optional<int> active_text_box;
        };

        glm::vec3 color;

        std::optional<Popup> popup;
        std::optional<CenteredBox> port_widget_box;
    };

    struct None
    {};
}

using Widget = std::variant<Widgets::TextBox, Widgets::ColorPicker, Widgets::None>;
using WidgetState = std::variant<Widgets::TextBoxState, Widgets::ColorPickerState, Widgets::None>;

#endif //GIE_WIDGET_H
