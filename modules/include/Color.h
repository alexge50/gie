//
// Created by alex on 5/23/19.
//

#ifndef MODULES_COLOR_H
#define MODULES_COLOR_H

#include <cstdint>

struct Color
{
    Color(): r{0}, g{0}, b{0} {}
    Color(uint8_t r_, uint8_t g_, uint8_t b_): r{r_}, g{g_}, b{b_} {}
    Color(const Color&) = default;
    Color(Color&&) = default;

    uint8_t r, g, b;
};

#endif //MODULES_COLOR_H
