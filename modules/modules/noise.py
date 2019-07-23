import modules.internals

def perlin_noise(source: Image, seed: int, zoom: float, z: float, octaves: int, persistence: float)->Image:
    return modules.images_internal.perlin_noise(source, seed, zoom, z, octaves, persistence)

def noise(source: Image, seed: int) -> Image:
    return modules.images_internal.noise(source, seed)