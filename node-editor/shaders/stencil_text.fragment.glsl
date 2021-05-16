#version 330 core

in vec2 uv_coordinates;
in vec2 vertex_position;

out vec4 Color;

uniform sampler2D text;
uniform vec4 color;
uniform vec2 bounding_box_upper_left;
uniform vec2 bounding_box_bottom_right;

void main()
{
    bvec2 a = greaterThanEqual(vertex_position, vec2(bounding_box_upper_left.x, bounding_box_bottom_right.y));
    bvec2 b = greaterThanEqual(vec2(bounding_box_bottom_right.x, bounding_box_upper_left.y), vertex_position);
    float opacity = float(a.x && a.y && b.x && b.y);
    vec4 sample = vec4(1.0, 1.0, 1.0, texture(text, uv_coordinates).r * opacity);
    Color = vec4(color) * sample;
}
