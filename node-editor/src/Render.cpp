#include <Render.h>
#include <RenderState.h>
#include <detail/Compute.h>
#include <detail/Zip.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const float OUTLINE_Z_LOCATION = 2.f;
static const float BASE_Z_LOCATION = 0.f;
static const float PORT_Z_LOCATION = 3.f;
static const float PORT_OUTLINE_Z_LOCATION = 4.f;
static const float HEADER_Z_LOCATION = 1.f;
static const float STRIDE_Z_LOCATION = 5.f;

static const int CIRCLE_POINTS = 64;

template<int N>
constexpr std::array<glm::vec2, N + 1> generate_circle()
{
    std::array<glm::vec2, N + 1> r{};

    for(int i = 0; i <= N; i++)
    {
        r[i] = {
                0.5f * cosf( 2.f * M_PI * i / N ),
                0.5f * sinf( 2.f * M_PI * i / N ),
        };
    }

    return r;
}

static void compute_render_state(NodeEditor& node_editor)
{
    auto z = static_cast<float>(node_editor.focus_stack.size());

    for(const auto node_id: node_editor.focus_stack)
    {
        node_editor.render_state.node_position_z[node_id] = z * STRIDE_Z_LOCATION;
        z--;
    }
}

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

    circle = create(
            generate_circle<CIRCLE_POINTS>()
    );

    line = create<2>({
        glm::vec2{0.f, 0.f},
        glm::vec2{1.f, 1.f},
    });
}

Render::~Render()
{

}

void Render::operator()(NodeEditor &node_editor)
{
    compute_render_state(node_editor);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    glm::mat4 view_projection = world_space_mat(node_editor.camera);

    for(const auto& connection: node_editor.graph.connections)
    {
        const glm::vec2 point_a = node_editor.graph.nodes.at(connection.input_port.node_id).position +
                node_editor.graph.node_types_computed.at(
                    node_editor.graph.nodes.at(connection.input_port.node_id).node_type
                ).input_port_positions[connection.input_port.port_id];
        const glm::vec2 point_b = node_editor.graph.nodes.at(connection.output_port.node_id).position +
                node_editor.graph.node_types_computed.at(
                    node_editor.graph.nodes.at(connection.output_port.node_id).node_type
                ).output_port_positions[connection.output_port.port_id];

        glm::vec2 translate = point_a;
        glm::vec2 scale = point_b - point_a;

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(translate, 1.0f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(scale, 0.f));

        solid_shader.prepare({
            .mvp = view_projection * model,
            .color = glm::vec4(config.connection_color, 1.f)
        });

        glLineWidth(config.connection_width);
        glBindVertexArray(line.vao);
        glDrawArrays(
                GL_LINES,
                0,
                2
        );
    }

    glDisable(GL_LINE_SMOOTH);

    for(const auto& [id, node_]: node_editor.graph.nodes)
    {
        const Node& node = node_;
        const NodeTypeCompute& node_type_computed = node_editor.graph.node_types_computed.at(node.node_type);
        const NodeType& node_type = node_editor.graph.node_types.at(node.node_type);

        float node_position_z = node_editor.render_state.node_position_z.at(id);

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3(node.position, node_position_z + BASE_Z_LOCATION)) *
                glm::scale(glm::mat4(1.f), glm::vec3(node_type_computed.size, 1.f));

        glm::mat4 mvp = view_projection * model;

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
                glm::translate(glm::mat4(1.f), glm::vec3(node.position + node_type_computed.header_position, node_position_z + HEADER_Z_LOCATION)) *
                glm::scale(glm::mat4(1.f), glm::vec3(
                        node_type_computed.size.x,
                        config.header_height,
                        0.f
                        ));
        mvp = view_projection * model;

        solid_shader.prepare({
            .mvp = mvp,
            .color = glm::vec4(node_type.color, 1.f)
        });

        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );


        model =
                glm::translate(glm::mat4(1.f), glm::vec3(node.position, node_position_z + OUTLINE_Z_LOCATION)) *
                glm::scale(glm::mat4(1.f), glm::vec3(node_type_computed.size, 1.f));

        mvp = view_projection * model;

        glm::vec3 outline_color =
                !node_editor.input_state.selected_nodes.contains(id) ?
                    config.node_outline_color :
                    config.node_selected_outline_color;

        solid_shader.prepare({
            .mvp = mvp,
            .color = glm::vec4(outline_color, 1.f)
        });

        glLineWidth(config.node_outline_width);
        glBindVertexArray(quad_outline.vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );


        auto render_node = [&node, this, &view_projection, &outline_color, &node_position_z, &config](const auto& position, const auto& port)
        {
            glm::mat4 model =
                    glm::translate(glm::mat4(1.f), glm::vec3(node.position + position, node_position_z + PORT_Z_LOCATION)) *
                    glm::scale(glm::mat4(1.f), glm::vec3(10.f, 10.f, 1.f));

            glm::mat4 mvp = view_projection * model;

            solid_shader.prepare({
                                         .mvp = mvp,
                                         .color = glm::vec4(port.color, 1.f)
                                 });

            glBindVertexArray(circle.vao);
            glDrawArrays(
                    GL_POLYGON,
                    0,
                    CIRCLE_POINTS + 1
            );

            model =
                    glm::translate(glm::mat4(1.f), glm::vec3(node.position + position, node_position_z + PORT_OUTLINE_Z_LOCATION)) *
                    glm::scale(glm::mat4(1.f), glm::vec3(10.f, 10.f, 1.f));

            mvp = view_projection * model;

            solid_shader.prepare({
                                         .mvp = mvp,
                                         .color = glm::vec4(outline_color, 1.f)
                                 });

            glLineWidth(config.node_outline_width * 1.5f);
            glBindVertexArray(circle.vao);
            glDrawArrays(
                    GL_LINE_LOOP,
                    0,
                    CIRCLE_POINTS
            );
        };

        for(const auto&[position, port]:
                ConstZipObject{node_type_computed.input_port_positions, node_type.input_ports})
        {
            render_node(position, port);
        }

        for(const auto&[position, port]:
                ConstZipObject{node_type_computed.output_port_positions, node_type.output_ports})
        {
            render_node(position, port);
        }
    }
    glDisable(GL_DEPTH_TEST);

    if(std::holds_alternative<SelectDrag>(node_editor.input_state.drag_state))
    {
        auto& select_drag = std::get<SelectDrag>(node_editor.input_state.drag_state);

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