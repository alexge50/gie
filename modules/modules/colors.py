import modules.internals

def to_color(r: int, g: int, b: int) -> Color:
    return Color(r, g, b)

def invert(color: Color) -> Color:
    return Color(255 - color.r, 255 - color.g, 255 - color.b)

def red_channel(color: Color) -> int:
    return color.r

def green_channel(color: Color) -> int:
    return color.g

def blue_channel(color: Color) -> int:
    return color.b

def solid_color(source: Image, color: Color) -> Image:
    return modules.images_internal.solid_color(source, color)