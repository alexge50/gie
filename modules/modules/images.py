import modules.internals

def average_color(image: Image)->Color:
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

    return Color(r, g, b)

def sample(image: Image, row: int, column: int)->Color:
    return image.pixel_at(row, column)

def separate_blue_channel(image: Image)->Image:
    return modules.images_internal.separate_blue_channel(image)

def separate_red_channel(image: Image)->Image:
    return modules.images_internal.separate_red_channel(image)

def separate_green_channel(image: Image)->Image:
    return modules.images_internal.separate_green_channel(image)

def luminance_map(image: Image)->Image:
    return modules.images_internal.luminance_map(image)

def mask(source: Image, mask: Image)->Image:
    return modules.images_internal.mask(source, mask)

def add(a: Image, b: Image)->Image:
    return modules.images_internal.add(a, b)

def strict_add(a: Image, b: Image)->Image:
    return modules.images_internal.strict_add(a, b)

def multiply(a: Image, b: Image)->Image:
    return modules.images_internal.multiply(a, b)

def discriminate_greater_than(source: Image, threshold: int)->Image:
    return modules.images_internal.discriminator_greater_than(source, threshold)

def discriminate_lesser_than(source: Image, threshold: int)->Image:
    return modules.images_internal.discriminator_lesser_than(source, threshold)

def discriminate_range(source: Image, a: int, b:int)->Image:
    return modules.images_internal.discriminator_range(source, a, b)

def proxy(source: Image)->Image:
    return source
