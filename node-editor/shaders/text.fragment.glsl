#version 330 core

in vec2 uv_coordinates;

out vec4 Color;

uniform sampler2D text;
uniform vec4 color;

void main()
{
    vec4 sample = vec4(1.0, 1.0, 1.0, texture(text, uv_coordinates).r);
    Color = vec4(color) * sample;
}
