import modules.internals

def invert(color: modules.internals.Color) -> modules.internals.Color:
    return Color(r=1. - color.r, g=1. - color.g, b=1. - color.b)