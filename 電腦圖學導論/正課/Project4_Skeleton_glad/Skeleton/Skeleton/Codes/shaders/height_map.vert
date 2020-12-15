#version 430 core
#define pi = 3.14159;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;


uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform sampler2D texture_diffuse1;
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
    vec3 height_map = position;
    float tmp_height = (texture(texture_diffuse1,texture_coordinate/wavelength).r-0.5f) * amplitude;
    float tmp_interactive = 0.0f;
    if(drop_point.x >0.0f){        
        float dist = distance(texture_coordinate, drop_point) / interactive_wavelength * 100;
        float t_c = (time-drop_time)*(interactive_radius*3.1415926)*interactive_speed;
        tmp_interactive = interactive_amplitude * sin((dist-t_c)*clamp(0.0125*t_c,0,1))/(exp(0.1*abs(dist-t_c)+(0.05*t_c)))*1.5;
    }
    if((tmp_height <0 && tmp_interactive >0)||(tmp_height >0 && tmp_interactive <0)){
        height_map.y += (tmp_height + tmp_interactive);
    }
    else{
        height_map.y += (abs(tmp_height) > abs(tmp_interactive)) ? tmp_height:tmp_interactive;
    }
    gl_Position = projection * view * model * vec4(height_map, 1.0f);
    v_out.position = vec3(model * vec4(height_map, 1.0));
    v_out.normal = mat3(transpose(inverse(model))) * normal;
    v_out.texture_coordinate = texture_coordinate;
}
