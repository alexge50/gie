import modules.internals

def to_color(r: float, g: float, b: float) -> modules.internals.Color:
    return modules.internals.Color(r, g, b)

def invert(color: modules.internals.Color) -> modules.internals.Color:
    return modules.internals.Color(1. - color.r, 1. - color.g, 1. - color.b)

def red_channel(color: modules.internals.Color) -> float:
    return color.r

def green_channel(color: modules.internals.Color) -> float:
    return color.g

def blue_channel(color: modules.internals.Color) -> float:
    return color.b