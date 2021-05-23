#version 330 core

in vec2 uv_coordinates;

out vec4 Color;

void main()
{
    float luminance = max(1. - uv_coordinates.y, 0.);

    Color = vec4(vec3(luminance, luminance, luminance), 1.);
}