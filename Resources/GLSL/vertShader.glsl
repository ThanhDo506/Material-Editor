#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec3 aTangent;

out vec2 TexCoords;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aTexCoord;
	Color = aColor;
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}