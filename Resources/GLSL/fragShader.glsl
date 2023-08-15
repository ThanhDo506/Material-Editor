#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec4 Color;

uniform sampler2D Diffuse;
uniform sampler2D Specular;

void main()
{
    FragColor = mix(texture(Diffuse, TexCoords), texture(Specular, TexCoords), 0.9);
}

