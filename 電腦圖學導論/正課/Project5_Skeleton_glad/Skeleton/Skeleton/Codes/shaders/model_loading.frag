#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 worldFragPos;

uniform sampler2D texture_diffuse1;

#pragma include fog.glsl
struct Fog{
    float Density;
    vec3 Color;
};

float getFogFactor(Fog fog, vec3 viewPos, vec3 fragPos){
    float distance = length(viewPos - fragPos);
    float factor = exp(-pow(fog.Density * distance, 2));
    factor = clamp(factor, 0.0, 1.0);
    return 1.0 - factor;
}

uniform Fog fog;
uniform vec3 viewPos;

void main()
{    
    vec4 color = texture(texture_diffuse1, TexCoords);
    float fogFactor = getFogFactor(fog, viewPos, worldFragPos);
    vec3 colorWithFog = mix(color.rgb, fog.Color, fogFactor);

    FragColor = vec4(colorWithFog, color.a);
   
}