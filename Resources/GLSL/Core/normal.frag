#pragma once

vec3 sampleNormalMap(sampler2D normalMap, vec2 texCoords) {
    return normalize(texture(normalMap, texCoords).xyz * 2.0 - 1.0);
}

vec4 normalColor(vec3 normal) { return vec4((normal + 1.0) / 2.0, 1.0); }

mat3 getTBN(vec3 tangent, vec3 normal, mat4 transform) {
    vec3 T = normalize(vec3(transform * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(transform * vec4(normal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    return mat3(T, B, N);
}