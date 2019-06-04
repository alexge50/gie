import modules.internals
import modules.images_internal

def pixel_sort(source: modules.internals.Image, mask: modules.internals.Image)->modules.internals.Image:
    return modules.images_internal.pixel_sort(source, mask)

def pixel_distort_displace(source: modules.internals.Image, map: modules.internals.Image, row_factor: float, column_factor: float)->modules.internals.Image:
    return modules.images_internal.pixel_distort_displace(source, map, row_factor, column_factor)

def displacement(source: modules.internals.Image, map: modules.internals.Image, row_factor: float, column_factor: float)->modules.internals.Image:
    return modules.images_internal.displacement(source, map, row_factor, column_factor)
