#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 model_view;
uniform mat4 u_model;
uniform mat4 projection;
uniform float amplitude;

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} v_out;

void main()
{
    gl_Position = projection * model_view * vec4(position.x,(sin(position.x*50)*3.14159 /180)* amplitude,position.z, 1.0f);
    v_out.position = vec3(u_model * vec4(position, 1.0f));
    //gl_Position.y = sin(gl_Position.x) * amplitude;
    v_out.position.y = sin(v_out.position.x*100) * amplitude;
    v_out.normal = mat3(transpose(inverse(u_model))) * normal;
    v_out.texture_coordinate = vec2(texture_coordinate.x, 1.0f - texture_coordinate.y);

}



//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;
//
//out vec2 TexCoords;
//
//uniform mat4 model_view;
////uniform mat4 view;
//uniform mat4 projection;
//
//void main()
//{
//    TexCoords = aTexCoords;    
//    gl_Position = projection * model_view * vec4(aPos, 1.0);
//}