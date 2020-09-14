#version 330 core

in vec2 fragment_position;

out vec4 Color;

//uniform float scale = 1.f;

const float scale = 1.f;

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
    float col = grid(fragment_position.xy, 20.f * scale, 2.f * scale) + grid(fragment_position.xy, 100.f * scale, 4.f * scale);

    Color = vec4(vec3(col) * 0.5f, 1.0);
}