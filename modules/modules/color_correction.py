import modules.internals
import modules.images_internal

def lift_gain(source: modules.internals.Image, lift: float, gain: float)->modules.internals.Image:
    return modules.images_internal.lift_gain(source, lift, gain)

def gamma(source: modules.internals.Image, gamma: float)->modules.internals.Image:
    return modules.images_internal.gamma(source, gamma)

def contrast(source: modules.internals.Image, contrast: float)->modules.internals.Image:
    return modules.images_internal.contrast(source, contrast)

def brightness(source: modules.internals.Image, brightness: float)->modules.internals.Image:
    return modules.images_internal.brightness(source, brightness)