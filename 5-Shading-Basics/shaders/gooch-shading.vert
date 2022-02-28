#version 450

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_norm;
layout(location = 2) in vec2 a_uv;

void main()
{
    gl_Position = vec4(a_pos, 1.0f);
}