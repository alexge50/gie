#ifndef NODE_EDITOR_BOUNDINGBOX_H
#define NODE_EDITOR_BOUNDINGBOX_H

#include <glm/vec2.hpp>

struct BoundingBox{
    glm::vec2 upper_left;
    glm::vec2 bottom_right;
};

static BoundingBox compute_bounding_box(glm::vec2 position, glm::vec2 size)
{
    return {
            .upper_left = position - size / 2.f,
            .bottom_right = position + size / 2.f
    };
}

static bool contains_point(BoundingBox box, glm::vec2 point)
{
    return box.upper_left.x <= point.x && box.bottom_right.x >= point.x &&
           box.upper_left.y <= point.y && box.bottom_right.y >= point.y;
}

#endif //NODE_EDITOR_BOUNDINGBOX_H
