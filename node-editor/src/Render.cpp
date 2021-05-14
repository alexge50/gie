#include <Render.h>
#include <GraphCache.h>
#include <detail/Compute.h>
#include <detail/Zip.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

void Render::operator()(const GraphCache& graph_cache, const StylingConfig& config)
{
    glm::vec2 screen_size = graph_cache.camera.screen_size;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));
        glm::mat4 model =
                glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));

        glm::mat4 mvp = background_mat(graph_cache.camera) * model;

        glBindVertexArray(quad.vao);
        background_shader.prepare({
            .mvp = mvp,
            .model = model,
            .scale = scale,
            .camera_position = graph_cache.camera.position,
            .zoom = graph_cache.camera.zoom,
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

    glm::mat4 view_projection = world_space_mat(graph_cache.camera);

    for(const auto& connection: graph_cache.connections)
    {
        glm::vec3 translate = connection.source_position;
        glm::vec3 scale = connection.destination_position - connection.source_position;

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), translate) *
                glm::scale(glm::mat4(1.f), scale);

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

    for(const auto& node: graph_cache.nodes)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), node.position) *
                glm::scale(glm::mat4(1.f), node.size);

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
    }

    for(const auto& node_header: graph_cache.node_headers)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), node_header.position) *
                glm::scale(glm::mat4(1.f), node_header.size);
        glm::mat4 mvp = view_projection * model;

        solid_shader.prepare({
                                     .mvp = mvp,
                                     .color = glm::vec4(node_header.color, 1.f)
                             });

        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );
    }

    for(const auto& node_outline: graph_cache.node_outlines)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), node_outline.position) *
                glm::scale(glm::mat4(1.f), node_outline.size);

        glm::mat4 mvp = view_projection * model;

        glm::vec3 outline_color =
                !node_outline.selected ?
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
    }

    for(const auto& port: graph_cache.ports)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), port.position) *
                glm::scale(glm::mat4(1.f), glm::vec3(config.port_radius, config.port_radius, 1.f));

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
    }

    for(const auto& port_outline: graph_cache.port_outline)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), port_outline.position) *
                glm::scale(glm::mat4(1.f), glm::vec3(config.port_radius, config.port_radius, 1.f));

        glm::mat4 mvp = view_projection * model;

        glm::vec3 outline_color =
                !port_outline.selected ?
                config.node_outline_color :
                config.node_selected_outline_color;

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
    }

    glDisable(GL_DEPTH_TEST);

    if(graph_cache.select_box)
    {
        auto box = graph_cache.select_box->box;

        glm::vec2 box_size = {
            fabsf(box.upper_left.x - box.bottom_right.x),
            fabsf(box.upper_left.y - box.bottom_right.y)
        };

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3((box.upper_left + box.bottom_right) / 2.f, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(box_size, 0.f));

        glm::mat4 mvp = screen_projection_mat(graph_cache.camera) * model;

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