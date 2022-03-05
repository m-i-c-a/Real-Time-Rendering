#version 450

layout(location = 0) out vec4 out_color;

in vec3 v_pos;
in vec3 v_normal;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse; 
    vec3 specular;
};
uniform DirLight u_dir_light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_mat;

uniform vec3 u_view_pos;

vec3 CalcDirLight(in vec3 normal);
vec3 CalcPointLight();

void main() {
    vec3 normal = normalize(v_normal);

    vec3 I = CalcDirLight(normal);

    out_color = vec4(I, 1.0f);
}

vec3 CalcDirLight(in vec3 normal) {
    vec3 frag_to_light = normalize(-u_dir_light.direction);
    vec3 frag_to_view = normalize(u_view_pos - v_pos);

    float diff = max(0.0f, dot(normal, frag_to_light));

    float spec = 0.0f;
    if (diff > 0) {

        // reflect(...) expects arg1 light->frag, so we negate "frag_to_light" to get light->frag
        vec3 reflect_dir = reflect(-frag_to_light, normal);
        spec = pow(max(0.0f, dot(reflect_dir, frag_to_view)), u_mat.shininess);
    }

    vec3 ambient  = u_dir_light.ambient * u_mat.ambient;
    vec3 diffuse  = u_dir_light.diffuse * (diff * u_mat.diffuse);
    vec3 specular = u_dir_light.specular * (spec * u_mat.specular);

    return (ambient + diffuse + specular);
}