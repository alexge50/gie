import modules.internals

def perlin_noise(source: Image, seed: int, z: float, octaves: int, persistence: float)->Image:
    return modules.images_internal.perlin_noise(source, seed, z, octaves, persistence)

def distributed_noise(source: Image) -> Image:
    return to_image(np.random.rand(source.height(), source.width(), 3))