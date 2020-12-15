#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform float interactive_amplitude,wavelength,time,speed,interactive_wavelength,interactive_speed,interactive_radius;

uniform vec2 drop_point;
uniform float drop_time;

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} v_out;

void main()
{
   // float k = 2 * 3.1415926535 * frequency;
   // float w =  k * (position.x)+t;

   vec3 p = position;
   float dist = distance(texture_coordinate, drop_point) / interactive_wavelength * 50;
   float t_c = (time-drop_time)*(interactive_radius*3.1415926)*interactive_speed;
   if(drop_point.x >0.0f){
    p.y += interactive_amplitude * sin((dist-t_c)*clamp(0.0125*t_c,0,1))/(exp(0.1*abs(dist-t_c)+(0.05*t_c)))*1.5;
   }
   // vec3 tangent = normalize(vec3(1,k*amplitude*cos(w),0));
   // v_out.normal =  mat3(transpose(inverse(model_matrix)))*normalize(vec3(-tangent.y, tangent.x, 0));
    v_out.normal = mat3(transpose(inverse(model)))*normal;
    gl_Position = projection * view * model * vec4(p, 1.0f);
    v_out.position = vec3(model * vec4(p, 1.0));
    v_out.texture_coordinate = texture_coordinate;

}