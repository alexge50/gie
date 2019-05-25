import modules.internals

def average_color(image: modules.internals.Image)->modules.internals.Color:
    r = 0
    g = 0
    b = 0
    pixel_count = image.width() * image.height()

    for row in range(image.height()):
        for column in range(image.width()):
            color = image.pixel_at(row, column)
            r += color.r
            g += color.g
            b += color.b

    r = r // pixel_count
    g = g // pixel_count
    b = b // pixel_count

    return modules.internals.Color(r, g, b)

def sample(image: modules.internals.Image, row: int, column: int)->modules.internals.Color:
    return image.pixel_at(row, column)