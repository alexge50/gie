#ifndef NODE_EDITOR_CAMERA_H
#define NODE_EDITOR_CAMERA_H

#include "detail/ArithmeticStrongAlias.h"

#include <glm/glm.hpp>

using ScreenSpaceCoordinates = ArithmeticStrongAlias<glm::vec2, struct ScreenSpaceCoordinatesTag>;
using WorldSpaceCoordinates = ArithmeticStrongAlias<glm::vec2, struct WorldSpaceCoordinatesTag>;

struct Camera
{
    float zoom = 1.f;
    glm::vec2 position {0.f, 0.f};
    glm::vec2 screen_size;
};

WorldSpaceCoordinates to_world_space(const Camera&, ScreenSpaceCoordinates);
glm::mat4 world_space_mat(const Camera&); // matrix that converts from world space to screen space
glm::mat4 background_mat(const Camera&); // matrix for rendering the background
glm::mat4 screen_projection_mat(const Camera&); // used for objects rendered over the editor, in screen space


#endif //NODE_EDITOR_CAMERA_H
