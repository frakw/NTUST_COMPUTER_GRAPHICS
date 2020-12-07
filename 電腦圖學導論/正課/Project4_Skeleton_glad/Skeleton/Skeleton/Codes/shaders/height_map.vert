#version 430 core
#define pi = 3.14159;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform sampler2D texture_diffuse1;
uniform float amplitude,wavelength,time,speed;

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} v_out;

void main()
{
    vec3 height_map = position;
    height_map.y = height_map.y + (texture(texture_diffuse1,texture_coordinate/wavelength).r-0.5f) * amplitude * 5.0f;
    gl_Position = projection * view * model * vec4(height_map, 1.0f);
    v_out.position = vec3(model * vec4(height_map, 1.0));;
    v_out.normal = mat3(transpose(inverse(model))) * normal;
    v_out.texture_coordinate = texture_coordinate;

}
