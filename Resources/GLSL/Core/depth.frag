#pragma once

float linearizeDepth(float depth, float near, float far) {
    float normalizedDepth = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - normalizedDepth * (far - near));
}

vec4 depthColor(float near, float far) {
    float depth = linearizeDepth(gl_FragCoord.z, near, far);
    float depthColor = depth / far;
    return vec4(vec3(depthColor), 1.0);
}