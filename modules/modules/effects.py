import modules.internals

def pixel_sort(source: Image, mask: Image)->Image:
    return modules.images_internal.pixel_sort(source, mask)

def pixel_distort_displace(source: Image, map: Image, row_factor: float, column_factor: float)->Image:
    return modules.images_internal.pixel_distort_displace(source, map, row_factor, column_factor)

def displacement(source: Image, map: Image, row_factor: float, column_factor: float)->Image:
    return modules.images_internal.displacement(source, map, row_factor, column_factor)

def lens_distortion(source: Image, strength: float, zoom: float) -> Image:
    return modules.images_internal.lens_distortion(source, strength, zoom)
