class Color:
    def __init__(self, r: float, g: float, b: float):
        (self.r, self.g, self.b) = (r, g, b)

def import_globals():
    globals()['Color'] = Color