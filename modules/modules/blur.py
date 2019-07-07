import modules.images_internal

def box_blur(source: Image, row_factor: float, column_factor: float)->Image:
    return modules.images_internal.box_blur(source, row_factor, column_factor)