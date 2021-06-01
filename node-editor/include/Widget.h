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

    struct RealTextBox
    {
        std::optional<double> min_bound = std::nullopt;
        std::optional<double> max_bound = std::nullopt;
    };

    struct IntegerTextBox
    {
        std::optional<long long> min_bound = std::nullopt;
        std::optional<long long> max_bound = std::nullopt;
    };

    struct TextBoxState
    {
        std::string text{};
        std::variant<std::string, long long, double> data = std::string{};
        size_t cursor_position = 0;
        float view_position = 0.f;

        CenteredBox text_box;
        bool invalid;
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

using Widget = std::variant<Widgets::TextBox, Widgets::IntegerTextBox, Widgets::RealTextBox, Widgets::ColorPicker, Widgets::None>;
using WidgetState = std::variant<Widgets::TextBoxState, Widgets::ColorPickerState, Widgets::None>;

#endif //GIE_WIDGET_H
