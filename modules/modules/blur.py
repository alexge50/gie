import modules.internals
import modules.images_internal

def box_blur(source: modules.internals.Image, row_factor: float, column_factor: float)->modules.internals.Image:
    return modules.images_internal.box_blur(source, row_factor, column_factor)