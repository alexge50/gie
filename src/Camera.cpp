#include <Camera.h>

#include <glm/gtc/matrix_transform.hpp>

WorldSpaceCoordinates to_world_space(const Camera& camera, ScreenSpaceCoordinates position)
{
    const auto view = glm::lookAt(
            glm::vec3(camera.position, 1.f),
            glm::vec3(camera.position, 0.f),
            glm::vec3(0.f, 1.f, 0.f));

    position -= camera.screen_size / 2.f;

    return glm::vec2{(1.f / camera.zoom) * glm::inverse(view) * glm::vec4(position.get(), 0.f, 1.f)};
}

glm::mat4 world_space_mat(const Camera& camera)
{
    const glm::mat4 projection = glm::ortho(
            -float(camera.screen_size.x) / 2.f, float(camera.screen_size.x) / 2.f,
            float(camera.screen_size.y) / 2.f, -float(camera.screen_size.y) / 2.f, -1.f, 1.f);

    const glm::mat4 view = glm::lookAt(
            glm::vec3(camera.position, 1.f),
            glm::vec3(camera.position, 0.f),
            glm::vec3(0.f, 1.f, 0.f)
    );

    const glm::mat4 zoom = glm::scale(glm::mat4(1.f), glm::vec3(camera.zoom, camera.zoom, 0.f));

    return projection * view * zoom;
}

glm::mat4 background_mat(const Camera& camera)
{
    return glm::ortho(
            -float(camera.screen_size.x) / 2.f, float(camera.screen_size.x) / 2.f,
            float(camera.screen_size.y) / 2.f, -float(camera.screen_size.y) / 2.f, -1.f, 1.f);
}

glm::mat4 screen_projection_mat(const Camera& camera)
{
    return glm::ortho(0.f, float(camera.screen_size.x), float(camera.screen_size.y), 0.f);
}