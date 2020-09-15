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
            -0.5f, 0.5f,
            0.5f, 0.5f,
            -0.5f, -0.5f,
            0.5f, -0.5f,
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
            -0.5f, 0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f,
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
    background_background_location = background_shader.getUniformLocation("background");
    background_foreground_location = background_shader.getUniformLocation("foreground");

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
    const auto& config = node_editor.styling_config;

    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(float(screen_size.x) / 2.f, float(screen_size.y) / 2.f, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));
        glm::mat4 projection = glm::ortho(0.f, float(screen_size.x), float(screen_size.y), 0.f);

        glm::mat4 mvp = projection * model;

        background_shader.use();
        glUniformMatrix4fv(background_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniformMatrix4fv(background_model_location, 1, 0, glm::value_ptr(model));
        glUniform1f(background_scale_location, node_editor.zoom);
        glUniform3f(background_background_location, config.grid_background.r, config.grid_background.g,
                    config.grid_background.b);
        glUniform3f(background_foreground_location, config.grid_foreground.r, config.grid_foreground.g,
                    config.grid_foreground.b);

        glBindVertexArray(quad_vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );
    }

    solid_shader.use();
    for(const auto& [id, node]: node_editor.graph.nodes)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(node.position, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(node_editor.graph.nodes_computed.at(id).size, 0.f));
        glm::mat4 projection = glm::ortho(0.f, float(screen_size.x), float(screen_size.y), 0.f);

        glm::mat4 mvp = projection * model;
        glm::mat4 node_mvp = mvp;
        solid_shader.use();
        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniform4f(solid_color_location, config.node_background_color.r, config.node_background_color.g, config.node_background_color.b, 1.f);

        glBindVertexArray(quad_vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );

        model =
                glm::translate(glm::mat4(1.f), glm::vec3(node_editor.graph.nodes_computed.at(id).header_position, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(
                        node_editor.graph.nodes_computed.at(id).size.x,
                        config.header_height,
                        0.f
                        ));
        mvp = projection * model;
        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniform4f(solid_color_location, node.color.r, node.color.g, node.color.b, 1.f);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );


        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(node_mvp));
        glUniform4f(solid_color_location, config.node_outline_color.r, config.node_outline_color.g, config.node_outline_color.b, 1.f);
        glLineWidth(config.node_outline_width);
        glBindVertexArray(quad_outline_vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );

    }
}
