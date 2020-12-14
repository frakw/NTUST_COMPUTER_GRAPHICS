#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube tiles;

void main()
{
    color = texture(tiles, TexCoords);
}