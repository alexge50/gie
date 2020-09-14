#ifndef NODE_EDITOR_NODEEDITOR_H
#define NODE_EDITOR_NODEEDITOR_H

#include <Graph.h>
#include <glm/vec2.hpp>

struct NodeEditor
{
    Graph graph;

    float zoom = 2.f;
    glm::vec2 camera_position;
};

#endif //NODE_EDITOR_NODEEDITOR_H
