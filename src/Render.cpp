#include <Render.h>

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

        glBindVertexArray(quad.vao);
        background_shader.prepare({
            .mvp = mvp,
            .model = model,
            .scale = scale,
            .camera_position = node_editor.camera.position,
            .zoom = node_editor.camera.zoom,
            .foreground = config.grid_foreground,
            .background = config.grid_background,
        });

        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );
    }

    glm::mat4 view_projection = world_space_mat(node_editor.camera);
    for(const auto& [id, node]: node_editor.graph.nodes)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(node.position, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(node_editor.graph.nodes_computed.at(id).size, 0.f));

        glm::mat4 mvp = view_projection * model;
        glm::mat4 node_mvp = mvp;

        solid_shader.prepare({
            .mvp = mvp,
            .color = glm::vec4(config.node_background_color, 1.f)
        });

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

        solid_shader.prepare({
            .mvp = mvp,
            .color = glm::vec4(node.color, 1.f)
        });

        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );


        glm::vec3 outline_color =
                !node_editor.selected_nodes.contains(id) ?
                    config.node_outline_color :
                    config.node_selected_outline_color;

        solid_shader.prepare({
            .mvp = node_mvp,
            .color = glm::vec4(outline_color, 1.f)
        });

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

        solid_shader.prepare({
            .mvp = mvp,
            .color = config.select_rectangle_color
        });

        glBindVertexArray(quad.vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );

        solid_shader.prepare({
            .mvp = mvp,
            .color = config.select_rectangle_outline_color
        });
        glBindVertexArray(quad_outline.vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );
    }
}