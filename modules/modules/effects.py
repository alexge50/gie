import modules.internals

import numpy as np
import scipy.signal as signal
import scipy.ndimage as ndimage

def pixel_sort(source: Image, mask: Image)->Image:
    return modules.images_internal.pixel_sort(source, mask)

def pixel_distort_displace(source: Image, map: Image, row_factor: float, column_factor: float)->Image:
    return modules.images_internal.pixel_distort_displace(source, map, row_factor, column_factor)

def displacement(source: Image, map: Image, row_factor: float, column_factor: float)->Image:
    return modules.images_internal.displacement(source, map, row_factor, column_factor)

def lens_distortion(source: Image, strength: float, zoom: float) -> Image:
    return modules.images_internal.lens_distortion(source, strength, zoom)

def sobel(source: Image) -> Image:
    a = to_ndarray(source)

    channels = []
    for d in range(3):
        channels.append(ndimage.sobel(a[:, :, d]))

    return to_image(np.stack(channels, axis=2))

def sobel_2(source: Image) -> Image:
    a = to_ndarray(source)

    sobel_x = np.c_[
        [-1, 0, 1],
        [-2, 0, 2],
        [-1, 0, 1]
    ]

    sobel_y = np.c_[
        [1, 2, 1],
        [0, 0, 0],
        [-1, -2, -1]
    ]

    channels = []
    for d in range(3):
        sx = signal.convolve2d(a[:, :, d], sobel_x, mode='same',
                               boundary='symm')
        sy = signal.convolve2d(a[:, :, d], sobel_y, mode='same',
                               boundary='symm')
        channels.append(np.sqrt(sx ** 2 + sy ** 2))

    return to_image(np.stack(channels, axis=2))