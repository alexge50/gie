#ifndef NODE_EDITOR_GRAPHCACHE_H
#define NODE_EDITOR_GRAPHCACHE_H

#include <glm/vec3.hpp>

#include <optional>
#include <vector>
#include <string_view>

#include <detail/BoundingArea.h>
#include <Camera.h>



struct RenderData
{
    struct Quad
    {
        glm::vec3 position;
        glm::vec3 size;
        glm::vec4 color;
    };

    struct QuadOutline
    {
        glm::vec3 position;
        glm::vec3 size;
        glm::vec4 color;
    };

    struct Circle
    {
        glm::vec3 position;
        float radius;
        glm::vec3 color;
    };

    struct CircleOutline
    {
        glm::vec3 position;
        float radius;
        glm::vec3 color;
    };

    struct Line
    {
        glm::vec3 source_position;
        glm::vec3 destination_position;
        glm::vec3 color;
    };

    struct Text
    {
        glm::vec3 position;
        glm::vec4 color;
        std::string_view text;
        float text_height;
    };

    struct StencilText
    {
        glm::vec3 position;
        glm::vec4 color;
        std::string_view text;
        float text_height;
        BoundingBox box;
    };

    struct CircleColorSpace
    {
        glm::vec3 position;
        glm::vec3 size;
        float luminance;
    };

    struct MonochromeGradient
    {
        glm::vec3 position;
        glm::vec3 size;
    };

    std::vector<Quad> quads;
    std::vector<QuadOutline> quad_outlines;
    std::vector<Circle> circles;
    std::vector<CircleOutline> circle_outlines;
    std::vector<Line> lines;
    std::vector<Text> texts;
    std::vector<StencilText> stencil_texts;
    std::vector<CircleColorSpace> circle_color_space;
    std::vector<MonochromeGradient> monochrome_gradient;

    float line_width;
    glm::vec3 background_color;
    glm::vec3 foreground_color;

    Camera camera{};
};

struct NodeEditor;
RenderData compute_render_data(const NodeEditor &node_editor);

#endif //NODE_EDITOR_GRAPHCACHE_H
