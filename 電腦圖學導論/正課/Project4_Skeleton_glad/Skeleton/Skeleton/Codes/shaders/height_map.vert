#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 model_view;
uniform mat4 u_model;
uniform mat4 projection;
uniform sampler2D texture_diffuse1;

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} v_out;

void main()
{
    vec3 height_map = position;
    height_map.y = height_map.y + texture(texture_diffuse1,texture_coordinate).r;
    gl_Position = projection * model_view * vec4(height_map, 1.0f);
    v_out.position = height_map;
    v_out.normal = mat3(transpose(inverse(u_model))) * normal;
    v_out.texture_coordinate = vec2(texture_coordinate.x, 1.0f - texture_coordinate.y);

}