#include <Render.h>
#include <detail/Shader.h>
#include <shaders.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Render::Render()
{
    glGenBuffers(1, &quad_vbo);
    glGenVertexArrays(1, &quad_vao);

    float background_quad[] = {
            0, 1.0,
            1.0, 1.0,
            0, 0,
            1.0, 0,
    };

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            8 * sizeof(float),
            background_quad,
            GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0
    );

    background_shader = createShader(
            std::string{reinterpret_cast<const char*>(background_vertex_glsl)},
            std::string{reinterpret_cast<const char*>(background_fragment_glsl)}).value();

    background_shader.use();
    background_mvp_location = background_shader.getUniformLocation("mvp");
    background_model_location = background_shader.getUniformLocation("model");
    background_scale_location = background_shader.getUniformLocation("scale");
}

Render::~Render()
{

}

void Render::operator()(const NodeEditor &node_editor, glm::vec2 screen_size)
{
    glm::mat4 model =
            glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));
    glm::mat4 projection = glm::ortho(0.f, float(screen_size.x), float(screen_size.y), 0.f);

    glm::mat4 mvp = projection * model;

    glUniformMatrix4fv(background_mvp_location, 1, 0, glm::value_ptr(mvp));
    glUniformMatrix4fv(background_model_location, 1, 0, glm::value_ptr(model));
    glUniform1f(background_scale_location, node_editor.zoom);

    background_shader.use();
    glDrawArrays(
            GL_TRIANGLE_STRIP,
            0,
            4
    );

}
