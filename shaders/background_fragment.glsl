#version 330 core

in vec2 fragment_position;

out vec4 Color;

uniform vec2 camera_position = vec2(0.f, 0.f);
uniform float scale = 1.f;

uniform vec3 foreground;
uniform vec3 background;

float grid(vec2 frag_coord, float spacing, float width)
{
    vec2 uv = frag_coord/vec2(spacing);

    float radius = width / 2.f;

    float x = float(abs(uv.x - floor(uv.x)) * spacing < radius) +
    float(abs(uv.x - ceil(uv.x)) * spacing < radius);
    float y = float(abs(uv.y - floor(uv.y)) * spacing < radius) +
    float(abs(uv.y - ceil(uv.y)) * spacing < radius);

    return clamp(x + y, 0., 1.);
}

void main()
{
    float col = grid(fragment_position.xy, 10.f * scale, 1.f * scale) + grid(fragment_position.xy, 1000.f * scale, 4.f * scale);
    col = clamp(col, 0., 1.);

    vec3 color = (1. - col) * background + col * foreground;

    Color = vec4(color, 1.0);
}