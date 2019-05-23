import modules.internals

Color = modules.internals.Color

def invert(color: Color) -> Color:
    return Color(r=1. - color.r, g=1. - color.g, b=1. - color.b)