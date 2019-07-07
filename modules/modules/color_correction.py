import modules.internals

def lift_gain(source: Image, lift: float, gain: float)->Image:
    return modules.images_internal.lift_gain(source, lift, gain)

def gamma(source: Image, gamma: float)->Image:
    return modules.images_internal.gamma(source, gamma)

def contrast(source: Image, contrast: float)->Image:
    return modules.images_internal.contrast(source, contrast)

def brightness(source: Image, brightness: float)->Image:
    return modules.images_internal.brightness(source, brightness)