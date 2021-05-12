#version 330 core

in vec2 fragment_position;

out vec4 Color;

uniform vec2 camera_position = vec2(0.f, 0.f);
uniform float zoom = 1.f;

uniform vec3 foreground;
uniform vec3 background;

//source: https://www.shadertoy.com/view/wdK3Dy
float grid(vec2 frag_coord, float space, float grid_width)
{
    vec2 p = frag_coord - vec2(.5f);
    vec2 size = vec2(grid_width - .5f);

    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;

    float g = min(a.x, a.y);
    return clamp(g, 0., 1.0);
}

void main()
{
    vec2 position = (camera_position + fragment_position.xy);
    float col = grid(position, 25.f * zoom, 1.f) * grid(position, 100.f * zoom, 1.5f);
    col = clamp(col, 0., 1.);

    vec3 color = (1. - col) * foreground + col * background;

    Color = vec4(color, 1.0);
}