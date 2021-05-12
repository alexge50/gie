#ifndef NODE_EDITOR_GEOMETRYBUFFER_H
#define NODE_EDITOR_GEOMETRYBUFFER_H

#include <array>
#include <glm/vec2.hpp>
#include <glad/glad.h>

struct GeometryBuffer
{
    unsigned int vao = 0;
    unsigned int vbo = 0;
};

template <std::size_t N>
GeometryBuffer create(const std::array<glm::vec2, N>& geometry)
{
    GeometryBuffer buffer;

    glGenBuffers(1, &buffer.vbo);
    glGenVertexArrays(1, &buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(glm::vec2) * geometry.size(),
            geometry.data(),
            GL_STATIC_DRAW
    );

    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

#endif //NODE_EDITOR_GEOMETRYBUFFER_H
