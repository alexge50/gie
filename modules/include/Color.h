//
// Created by alex on 5/23/19.
//

#ifndef MODULES_COLOR_H
#define MODULES_COLOR_H

struct Color
{
    Color(): r{0}, g{0}, b{0} {}
    Color(double r_, double g_, double b_): r{r_}, g{g_}, b{b_} {}
    Color(const Color&) = default;
    Color(Color&&) = default;

    double r, g, b;
};

#endif //MODULES_COLOR_H
