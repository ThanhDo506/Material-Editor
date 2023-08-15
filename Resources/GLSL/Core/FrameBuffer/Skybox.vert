#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 TexCoords;

uniform mat4 _View;
uniform mat4 _Projection;
uniform mat4 _Transform;

void main()
{
    TexCoords = aPosition;
    vec4 pos = _Projection * _View * _Transform * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}  