#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 model_view;
uniform mat4 model;
uniform mat4 projection;
uniform float amplitude,wavelength,time,speed;

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} v_out;

void main()
{
    float k = 2 * 3.14159 / wavelength;
    float f = k * (position.x - speed * time);
    vec3 sinwave = position;
    sinwave.y = amplitude * sin(f);
    vec3 tangent = normalize(vec3(1,k*amplitude*cos(f),0));
    v_out.normal = mat3(transpose(inverse(model))) *  normalize(vec3(-tangent.y, tangent.x, 0));// * normalize(vec3(-tangent.y, tangent.x, 0));
    gl_Position = projection * model_view * vec4(sinwave, 1.0f);
    v_out.position = vec3(model * vec4(sinwave, 1.0));//vec3(u_model * vec4(sinwave, 1.0f));
    //v_out.normal = mat3(transpose(inverse(model_view))) * normal;
    v_out.texture_coordinate = texture_coordinate;

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