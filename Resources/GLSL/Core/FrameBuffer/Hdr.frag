#version 330 core

#ifndef GAMMA
#define GAMMA 2.2
#endif

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform int hdr;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if(hdr > 0)
    {
        vec3 result;
        if(hdr == 1) {
            // reinhard
            result = hdrColor / (hdrColor + vec3(1.0));
        } else if (hdr == 2) {
            // exposure
            result = vec3(1.0) - exp(-hdrColor * exposure);
        }
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / GAMMA));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / GAMMA));
        FragColor = vec4(result, 1.0);
    }
}