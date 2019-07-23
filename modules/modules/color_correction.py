import modules.internals
import numpy as np

def lift_gain(source: Image, lift: float, gain: float)->Image:
    return modules.images_internal.lift_gain(source, lift, gain)

def gamma(source: Image, gamma: float)->Image:
    return modules.images_internal.gamma(source, gamma)

def contrast(source: Image, contrast: float)->Image:
    return modules.images_internal.contrast(source, contrast)

def brightness(source: Image, brightness: float)->Image:
    return modules.images_internal.brightness(source, brightness)

def remap(source: Image, a: float, b: float) -> Image:
    arr = to_ndarray(source)
    b = 0.001 if b == 0 else b
    return to_image(a + arr * (b - a) / b)
