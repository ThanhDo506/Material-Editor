﻿#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube _Skybox;

void main()
{    
    FragColor = texture(_Skybox, TexCoords);
}