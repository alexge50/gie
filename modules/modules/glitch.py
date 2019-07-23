import modules.internals

def jpeg_artifacts(source: Image, radius: int)->Image:
    return modules.images_internal.jpeg_artifacts(source, radius)