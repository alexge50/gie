import modules.internals
import modules.images_internal

def to_color(r: int, g: int, b: int) -> modules.internals.Color:
    return modules.internals.Color(r, g, b)

def invert(color: modules.internals.Color) -> modules.internals.Color:
    return modules.internals.Color(255 - color.r, 255 - color.g, 255 - color.b)

def red_channel(color: modules.internals.Color) -> int:
    return color.r

def green_channel(color: modules.internals.Color) -> int:
    return color.g

def blue_channel(color: modules.internals.Color) -> int:
    return color.b

def solid_color(source: modules.internals.Image, color: modules.internals.Color) -> modules.internals.Image:
    return modules.images_internal.solid_color(source, color)