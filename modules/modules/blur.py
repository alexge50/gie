import modules.images_internal

import numpy as np
import scipy.ndimage as ndimage

def box_blur(source: Image, row_factor: float, column_factor: float)->Image:
    return modules.images_internal.box_blur(source, row_factor, column_factor)

def gaussian_blur(source: Image, sigma: int) -> Image:
    a = to_ndarray(source)

    channels = []
    for d in range(3):
        channels.append(ndimage.gaussian_filter(a[:, :, d], sigma=sigma))

    return to_image(np.stack(channels, axis=2))