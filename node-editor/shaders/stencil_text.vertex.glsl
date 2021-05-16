#version 330 core

layout (location = 0) in vec2 Position;

out vec2 uv_coordinates;
out vec2 vertex_position;
uniform mat4 mvp;

void main()
{
    vec4 position = mvp * vec4(Position, 0.f, 1.f);
    uv_coordinates = Position + 0.5;
    vertex_position = position.xy;
    gl_Position = position;
}