#version 450

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_norm;
layout(location = 2) in vec2 a_uv;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

out vec3 v_normal;

void main()
{
    gl_Position = u_proj_matrix * u_view_matrix * vec4(a_pos, 1.0f);

    v_normal = a_norm;
}