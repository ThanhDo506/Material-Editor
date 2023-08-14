#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 _Transform;
uniform mat4 _CameraMtx;


void main()
{
    TexCoords = aTexCoords;    
    gl_Position = _CameraMtx * _Transform * vec4(aPos, 1.0);
}