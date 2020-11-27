//#version 430 core
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texture_coordinate;
//
//uniform mat4 u_model;
//
//layout (std140, binding = 0) uniform commom_matrices
//{
//    mat4 u_projection;
//    mat4 u_view;
//};
//
//out V_OUT
//{
//   vec3 position;
//   vec3 normal;
//   vec2 texture_coordinate;
//} v_out;
//
//void main()
//{
//    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
//
//    v_out.position = vec3(u_model * vec4(position, 1.0f));
//    v_out.normal = mat3(transpose(inverse(u_model))) * normal;
//    v_out.texture_coordinate = vec2(texture_coordinate.x, 1.0f - texture_coordinate.y);
//}
//

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}