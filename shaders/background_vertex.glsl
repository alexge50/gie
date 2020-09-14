#version 330 core

layout (location = 0) in vec2 Position;

out vec2 fragment_position;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    vec4 position = mvp * vec4(Position, 0.f, 1.f);
    fragment_position = (model * vec4(Position, 0.f, 1.f)).xy;
    gl_Position = position;
}