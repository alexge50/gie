import modules.internals
import modules.images_internal

def perlin_noise(source: modules.internals.Image, seed: int, z: float, octaves: int, persistence: float)->modules.internals.Image:
    return modules.images_internal.perlin_noise(source, seed, z, octaves, persistence)