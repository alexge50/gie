#include <Render.h>
#include <detail/Shader.h>
#include <shaders.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Render::Render()
{
    quad = create<4>({
        glm::vec2{-0.5f, 0.5f},
        glm::vec2{0.5f, 0.5f},
        glm::vec2{-0.5f, -0.5f},
        glm::vec2{0.5f, -0.5f},
    });

    quad_outline = create<4>({
        glm::vec2{-0.5f, 0.5f},
        glm::vec2{0.5f, 0.5f},
        glm::vec2{0.5f, -0.5f},
        glm::vec2{-0.5f, -0.5f},
    });

    background_shader = createShader(
            std::string{reinterpret_cast<const char*>(background_vertex_glsl)},
            std::string{reinterpret_cast<const char*>(background_fragment_glsl)}).value();

    background_shader.use();
    background_mvp_location = background_shader.getUniformLocation("mvp");
    background_model_location = background_shader.getUniformLocation("model");
    background_zoom_location = background_shader.getUniformLocation("zoom");
    background_scale_location = background_shader.getUniformLocation("scale");
    background_background_location = background_shader.getUniformLocation("background");
    background_foreground_location = background_shader.getUniformLocation("foreground");
    background_camera_position_location = background_shader.getUniformLocation("camera_position");

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

void Render::operator()(const NodeEditor &node_editor)
{
    const auto& config = node_editor.styling_config;

    glm::vec2 screen_size = node_editor.camera.screen_size;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));
        glm::mat4 model =
                glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));

        glm::mat4 mvp = background_mat(node_editor.camera) * model;

        background_shader.use();
        glUniformMatrix4fv(background_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniformMatrix4fv(background_model_location, 1, 0, glm::value_ptr(model));
        glUniformMatrix4fv(background_scale_location, 1, 0, glm::value_ptr(scale));
        glUniform1f(background_zoom_location, node_editor.camera.zoom);
        glUniform3f(background_background_location, config.grid_background.r, config.grid_background.g,
                    config.grid_background.b);
        glUniform3f(background_foreground_location, config.grid_foreground.r, config.grid_foreground.g,
                    config.grid_foreground.b);
        glUniform2f(background_camera_position_location, node_editor.camera.position.x, node_editor.camera.position.y);

        glBindVertexArray(quad.vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );
    }

    solid_shader.use();
    glm::mat4 view_projection = world_space_mat(node_editor.camera);
    for(const auto& [id, node]: node_editor.graph.nodes)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(node.position, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(node_editor.graph.nodes_computed.at(id).size, 0.f));

        glm::mat4 mvp = view_projection * model;
        glm::mat4 node_mvp = mvp;

        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniform4f(solid_color_location, config.node_background_color.r, config.node_background_color.g, config.node_background_color.b, 1.f);

        glBindVertexArray(quad.vao);
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
        mvp = view_projection * model;
        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniform4f(solid_color_location, node.color.r, node.color.g, node.color.b, 1.f);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );


        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(node_mvp));
        if (!node_editor.selected_nodes.contains(id))
            glUniform4f(solid_color_location, config.node_outline_color.r, config.node_outline_color.g, config.node_outline_color.b, 1.f);
        else glUniform4f(solid_color_location, config.node_selected_outline_color.r, config.node_selected_outline_color.g, config.node_selected_outline_color.b, 1.f);
        glLineWidth(config.node_outline_width);
        glBindVertexArray(quad_outline.vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );

    }

    if(std::holds_alternative<SelectDrag>(node_editor.drag_state))
    {
        auto& select_drag = std::get<SelectDrag>(node_editor.drag_state);

        glm::vec2 box_size = {
            fabsf(select_drag.current_corner.x - select_drag.begin_corner.x),
            fabsf(select_drag.current_corner.y - select_drag.begin_corner.y)
        };

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3((select_drag.current_corner + select_drag.begin_corner) / 2.f, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(box_size, 0.f));

        glm::mat4 mvp = screen_projection_mat(node_editor.camera) * model;

        glUniformMatrix4fv(solid_mvp_location, 1, 0, glm::value_ptr(mvp));
        glUniform4f(solid_color_location, config.select_rectangle_color.r, config.select_rectangle_color.g, config.select_rectangle_color.b, config.select_rectangle_color.a);
        glBindVertexArray(quad.vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );

        glBindVertexArray(quad_outline.vao);
        glUniform4f(solid_color_location, config.select_rectangle_outline_color.r, config.select_rectangle_outline_color.g, config.select_rectangle_outline_color.b, config.select_rectangle_outline_color.a);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );
    }
}
