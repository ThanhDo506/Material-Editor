#pragma once

#ifndef GAMMA
#define GAMMA 2.2
#endif

vec3 gammaCorrect(vec3 color, float gamma) {
    return pow(color, vec3(1.0 / gamma));
}
vec3 gammaCorrect(vec3 color) {
    return gammaCorrect(color, GAMMA);
}