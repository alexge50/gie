import modules.internals

def to_color(r: double, g:double, b: double) -> modules.internals.Color:
    return Color(r, g, b)

def invert(color: modules.internals.Color) -> modules.internals.Color:
    return Color(1. - color.r, 1. - color.g, 1. - color.b)

def red_channel(color: modules.internals.Color) -> double:
    return color.r

def green_channel(color: modules.internals.Color) -> double:
    return color.g

def blue_channel(color: modules.internals.Color) -> double:
    return color.b