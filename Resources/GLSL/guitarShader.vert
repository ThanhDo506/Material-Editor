#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texCoord;
layout (location = 4) in vec3 tangent;

out VS_OUT {
    vec3 CameraPosition;
    vec3 FragmentPosition;
    vec3 Normal;
    vec4 Color;
    vec2 TexCoord;
    vec3 Tangent;
    mat3 TBN;
    vec3 TangentViewPosition;
    vec3 TangentFragPosition;
} vs_out;

// MVP
uniform mat4 _ViewMatrix;
uniform mat4 _ProjectionMatrix;
uniform mat4 _TransformMatrix;

/*************************** NORMAL **************************/
mat3 getTBN(vec3 tangent, vec3 normal);
/*************************************************************/

void main() {
    vs_out.CameraPosition = inverse(_ViewMatrix)[3].xyz;
    vs_out.FragmentPosition = vec3(_TransformMatrix * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(_TransformMatrix))) * normal;
    vs_out.Color = color;
    vs_out.TexCoord = texCoord;
    vs_out.Tangent = tangent;
    vs_out.TBN = getTBN(tangent, normal);
    vs_out.TangentViewPosition = vs_out.TBN * vs_out.CameraPosition;
    vs_out.TangentFragPosition = vs_out.TBN * vec3(_TransformMatrix * vec4(position, 1.0));
    gl_Position = _ProjectionMatrix * _ViewMatrix * _TransformMatrix * vec4(position, 1.0);
}

mat3 getTBN(vec3 tangent, vec3 normal) {
    vec3 T = normalize(vec3(_TransformMatrix * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(_TransformMatrix * vec4(normal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    return mat3(T, B, N);
}