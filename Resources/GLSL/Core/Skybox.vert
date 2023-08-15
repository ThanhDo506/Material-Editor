#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 TexCoords;

uniform mat4 _View;
uniform mat4 _Projection

void main()
{
    vec4 pos = _Projection * _View * vec4(aPos, 1.0);
    TexCoords = aPosition;
    gl_Position = pos.xyww;
}  