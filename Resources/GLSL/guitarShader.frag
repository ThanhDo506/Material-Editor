#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D _Diffuse0;

void main()
{    
    FragColor = texture(_Diffuse0, TexCoords);
}