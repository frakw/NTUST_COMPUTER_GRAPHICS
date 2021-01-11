#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform sampler2D screenTexture;
out vec2 TexCoords;
out vec4 vertColor;

void main()
{
    TexCoords = aTexCoords;
    vertColor = texture2D(screenTexture,aTexCoords);
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  