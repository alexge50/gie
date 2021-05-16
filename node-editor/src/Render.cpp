#include <Render.h>
#include <RenderData.h>
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

Render::Render(const Font& font):
    font{&font}
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

    glGenTextures(128, glyph_textures);
    for(int i = 0; i < 128; i++)
    {
        Font::Glyph glyph = font.get_glyph(i);

        glBindTexture(GL_TEXTURE_2D, glyph_textures[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            glyph.width,
            glyph.height,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            glyph.data.data()
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //free(texture.data);
    }
}

Render::~Render()
{

}

void Render::operator()(const RenderData& render_data)
{
    glm::vec2 screen_size = render_data.camera.screen_size;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));
        glm::mat4 model =
                glm::scale(glm::mat4(1.f), glm::vec3(float(screen_size.x), float(screen_size.y), 0.f));

        glm::mat4 mvp = background_mat(render_data.camera) * model;

        glBindVertexArray(quad.vao);
        background_shader.prepare({
            .mvp = mvp,
            .model = model,
            .scale = scale,
            .camera_position = render_data.camera.position,
            .zoom = render_data.camera.zoom,
            .foreground = render_data.foreground_color,
            .background = render_data.background_color,
        });

        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    glm::mat4 view_projection = world_space_mat(render_data.camera);

    for(const auto& line_: render_data.lines)
    {
        glm::vec3 translate = line_.source_position;
        glm::vec3 scale = line_.destination_position - line_.source_position;

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), translate) *
                glm::scale(glm::mat4(1.f), scale);

        solid_shader.prepare({
            .mvp = view_projection * model,
            .color = glm::vec4(line_.color, 1.f)
        });

        glLineWidth(render_data.line_width);
        glBindVertexArray(line.vao);
        glDrawArrays(
                GL_LINES,
                0,
                2
        );
    }

    glDisable(GL_LINE_SMOOTH);

    for(const auto& quad_: render_data.quads)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), quad_.position) *
                glm::scale(glm::mat4(1.f), quad_.size);

        glm::mat4 mvp = view_projection * model;

        solid_shader.prepare({
                                     .mvp = mvp,
                                     .color = glm::vec4(quad_.color, 1.f)
                             });

        glBindVertexArray(quad.vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );
    }

    for(const auto& quad_outline_: render_data.quad_outlines)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), quad_outline_.position) *
                glm::scale(glm::mat4(1.f), quad_outline_.size);

        glm::mat4 mvp = view_projection * model;

        solid_shader.prepare({
                                     .mvp = mvp,
                                     .color = glm::vec4(quad_outline_.color, 1.f)
                             });

        glLineWidth(render_data.line_width);
        glBindVertexArray(quad_outline.vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );
    }

    for(const auto& circle_: render_data.circles)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), circle_.position) *
                glm::scale(glm::mat4(1.f), glm::vec3(circle_.radius, circle_.radius, 1.f));

        glm::mat4 mvp = view_projection * model;

        solid_shader.prepare({
                                     .mvp = mvp,
                                     .color = glm::vec4(circle_.color, 1.f)
                             });

        glBindVertexArray(circle.vao);
        glDrawArrays(
                GL_POLYGON,
                0,
                CIRCLE_POINTS + 1
        );
    }

    for(const auto& circle_outline_: render_data.circle_outlines)
    {
        glm::mat4 model =
                glm::translate(glm::mat4(1.f), circle_outline_.position) *
                glm::scale(glm::mat4(1.f), glm::vec3(circle_outline_.radius, circle_outline_.radius, 1.f));

        glm::mat4 mvp = view_projection * model;

        solid_shader.prepare({
                                     .mvp = mvp,
                                     .color = glm::vec4(circle_outline_.color, 1.f)
                             });

        glLineWidth(render_data.line_width * 1.5f);
        glBindVertexArray(circle.vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                CIRCLE_POINTS
        );
    }

    glDisable(GL_DEPTH_TEST);

    if(render_data.select_box)
    {
        auto box = render_data.select_box->box;

        glm::vec2 box_size = {
            fabsf(box.upper_left.x - box.bottom_right.x),
            fabsf(box.upper_left.y - box.bottom_right.y)
        };

        glm::mat4 model =
                glm::translate(glm::mat4(1.f), glm::vec3((box.upper_left + box.bottom_right) / 2.f, 0.f)) *
                glm::scale(glm::mat4(1.f), glm::vec3(box_size, 0.f));

        glm::mat4 mvp = screen_projection_mat(render_data.camera) * model;

        solid_shader.prepare({
            .mvp = mvp,
            .color = render_data.select_box->color
        });

        glBindVertexArray(quad.vao);
        glDrawArrays(
                GL_TRIANGLE_STRIP,
                0,
                4
        );

        solid_shader.prepare({
            .mvp = mvp,
            .color = render_data.select_box->outline_color
        });
        glBindVertexArray(quad_outline.vao);
        glDrawArrays(
                GL_LINE_LOOP,
                0,
                4
        );
    }

    glActiveTexture(GL_TEXTURE0);

    for(const auto& text: render_data.texts)
    {
        float scale = text.text_height / font->get_font_size();
        glm::vec2 position = text.position;

        for(char c: text.text)
        {
            float position_x = font->get_glyph(c).bearing.x * scale;
            float position_y = (-static_cast<float>(font->get_glyph(c).bearing.y) + static_cast<float>(font->get_glyph(c).height)) * scale;

            float width = font->get_glyph(c).width * scale;
            float height = font->get_glyph(c).height * scale;

            glm::mat4 model =
                    glm::translate(glm::mat4(1.f), glm::vec3(position + glm::vec2{position_x, position_y} + glm::vec2{width, -height} / 2.f, 1.f)) *
                    glm::scale(glm::mat4(1.f), glm::vec3(glm::vec2{width, height}, 0.f));

            glm::mat4 mvp = view_projection * model;

            text_shader.prepare({
                                        .mvp = mvp,
                                        .text = 0,
                                        .color = glm::vec4{1.f, 1.f, 1.f, 1.f},
                                });

            glBindTexture(GL_TEXTURE_2D, glyph_textures[int(c)]);
            glBindVertexArray(quad.vao);
            glDrawArrays(
                    GL_TRIANGLE_STRIP,
                    0,
                    4
            );

            position.x += (font->get_glyph(c).advance >> 6) * scale;
        }
    }

    for(const auto& text: render_data.stencil_texts)
    {
        float scale = text.text_height / font->get_font_size();
        glm::vec2 position = text.position;

        glm::vec2 box_upper_left = view_projection * glm::vec4{text.box.upper_left, 1.f, 1.f};
        glm::vec2 box_bottom_right = view_projection * glm::vec4{text.box.bottom_right, 1.f, 1.f};

        for(char c: text.text)
        {
            float position_x = font->get_glyph(c).bearing.x * scale;
            float position_y = (-static_cast<float>(font->get_glyph(c).bearing.y) + static_cast<float>(font->get_glyph(c).height)) * scale;

            float width = font->get_glyph(c).width * scale;
            float height = font->get_glyph(c).height * scale;

            glm::mat4 model =
                    glm::translate(glm::mat4(1.f), glm::vec3(position + glm::vec2{position_x, position_y} + glm::vec2{width, -height} / 2.f, 1.f)) *
                    glm::scale(glm::mat4(1.f), glm::vec3(glm::vec2{width, height}, 0.f));

            glm::mat4 mvp = view_projection * model;

            stencil_text_shader.prepare({
                                        .mvp = mvp,
                                        .text = 0,
                                        .color = glm::vec4{1.f, 1.f, 1.f, 1.f},
                                        .bounding_box_upper_left = box_upper_left,
                                        .bounding_box_bottom_right = box_bottom_right,
                                });

            glBindTexture(GL_TEXTURE_2D, glyph_textures[int(c)]);
            glBindVertexArray(quad.vao);
            glDrawArrays(
                    GL_TRIANGLE_STRIP,
                    0,
                    4
            );

            position.x += (font->get_glyph(c).advance >> 6) * scale;
        }
    }
}