#version 450

layout(location = 0) out vec4 out_color;

in vec3 v_pos;
in vec3 v_normal;

uniform vec3  u_dir_light_ray;
uniform float u_diffuse_factor;
uniform float u_ambient_factor;

void main()
{

    float I = u_diffuse_factor * u_ambient_factor + u_diffuse_factor * max(0.0f, dot(-u_dir_light_ray, v_normal));

    out_color = vec4(vec3(I), 1.0f);
}