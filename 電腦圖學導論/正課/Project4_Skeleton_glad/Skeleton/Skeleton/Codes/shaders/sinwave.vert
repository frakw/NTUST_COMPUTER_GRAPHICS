#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform float amplitude,wavelength,time,speed,interactive_amplitude,interactive_wavelength,interactive_speed,interactive_radius;

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
    float k = 2 * 3.14159 / wavelength;
    float f = k * (position.x - speed * time);
    vec3 sinwave = position;
    float tmp_height = amplitude * sin(f);
    float tmp_interactive = 0.0f;
    if(drop_point.x >0.0f){
        float dist = distance(texture_coordinate, drop_point) / interactive_wavelength*100;
        float t_c = (time-drop_time)*(interactive_radius*3.1415926)*interactive_speed;
        tmp_interactive = interactive_amplitude * sin((dist-t_c)*clamp(0.0125*t_c,0,1))/(exp(0.1*abs(dist-t_c)+(0.05*t_c)))*1.5;
    }
     if((tmp_height <0 && tmp_interactive >0)||(tmp_height >0 && tmp_interactive <0)){
        sinwave.y += (tmp_height + tmp_interactive);
    }
    else{
        sinwave.y += (abs(tmp_height) > abs(tmp_interactive)) ? tmp_height:tmp_interactive;
    }
    vec3 tangent = normalize(vec3(1,k*amplitude*cos(f),0));
    gl_Position = projection * view * model * vec4(sinwave, 1.0f);
    v_out.position = vec3(model * vec4(sinwave, 1.0));//vec3(u_model * vec4(sinwave, 1.0f));
    v_out.normal = mat3(transpose(inverse(model))) *  normalize(vec3(-tangent.y, tangent.x, 0));
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