#ifndef NODE_EDITOR_BOUNDINGBOX_H
#define NODE_EDITOR_BOUNDINGBOX_H

#include <glm/vec2.hpp>

struct BoundingBox
{
    glm::vec2 upper_left;
    glm::vec2 bottom_right;
};

struct Circle
{
    glm::vec2 center;
    float radius;
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

static bool contains_point(Circle circle, glm::vec2 point)
{
    return (point.x - circle.center.x) * (point.x - circle.center.x) +
           (point.y - circle.center.y) * (point.y - circle.center.y) <= circle.radius;
}

#endif //NODE_EDITOR_BOUNDINGBOX_H
