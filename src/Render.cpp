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

    float quad[] = {
            0, 1.0,
            1.0, 1.0,
            0, 0,
            1.0, 0,
    };

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            8 * sizeof(float),
            quad,
            GL_STATIC_DRAW
    );

    glBindVertexArray(quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &quad_outline_vbo);
    glGenVertexArrays(1, &quad_outline_vao);

    float quad_outline[] = {
            0, 1.0,
            1.0, 1.0,
            1.0, 0,
            0, 0,
            0, 1.0,
    };

    glBindBuffer(GL_ARRAY_BUFFER, quad_outline_vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            8 * sizeof(float),
            quad_outline,
            GL_STATIC_DRAW
    );

    glBindVertexArray(quad_outline_vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_outline_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    background_shader = createShader(
            std::string{reinterpret_cast<const char*>(background_vertex_glsl)},
            std::string{reinterpret_cast<const char*>(background_fragment_glsl)}).value();

    background_shader.use();
    background_mvp_location = background_shader.getUniformLocation("mvp");
    background_model_location = background_shader.getUniformLocation("model");
    background_scale_location = background_shader.getUniformLocation("scale");

    solid_shader = createShader(
            std::string{reinterpret_cast<const char*>(solid_vertex_glsl)},
            std::string{reinterpret_cast<const char*>(solid_fragment_glsl)}).value();

    solid_shader.use();
    solid_mvp_location = solid_shader.getUniformLocation("mvp");
    solid_color_location = solid_shader.getUniformLocation("color");
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

    background_shader.use();
    glUniformMatrix4fv(background_mvp_location, 1, 0, glm::value_ptr(mvp));
    glUniformMatrix4fv(background_model_location, 1, 0, glm::value_ptr(model));
    glUniform1f(background_scale_location, node_editor.zoom);

    glBindVertexArray(quad_vao);
    glDrawArrays(
            GL_TRIANGLE_STRIP,
            0,
            4
    );


    solid_shader.use();
    for(const auto& [id, node]: node_editor.graph.nodes)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(node.position, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(float(50.f), float(100.f), 0.f));
        glm::mat4 projection = glm::ortho(0.f, float(screen_size.x), float(screen_size.y), 0.f);

        glm::mat4 mvp = projection * model;
        solid_shader.use();
        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniform4f(solid_color_location, node.color.x, node.color.y, node.color.z, 1.f);

        glBindVertexArray(quad_vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );

    }
}
