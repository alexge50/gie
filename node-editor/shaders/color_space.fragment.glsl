#version 330 core

in vec2 uv_coordinates;

out vec4 Color;

// https://stackoverflow.com/a/17897228
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    float center_offset = sqrt(uv_coordinates.x * uv_coordinates.x + uv_coordinates.y * uv_coordinates.y);
    float theta = atan(uv_coordinates.y, uv_coordinates.x);
    float opacity = float(center_offset < 0.5);

    theta += 3.1415926535898;
    theta /= 2 * 3.1412926535898;

    Color = vec4(hsv2rgb(vec3(theta, center_offset / 0.5, 1.)), opacity);
}