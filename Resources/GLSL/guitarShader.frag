#version 330 core

const float PI = 3.14159265359;

#ifndef GAMMA
#define GAMMA 2.2
#endif

#ifndef MAX_DIFFUSE_TEXTURES
#define MAX_DIFFUSE_TEXTURES 1
#endif

#ifndef MAX_SPECULAR_TEXTURES
#define MAX_SPECULAR_TEXTURES 1
#endif

#ifndef MAX_ROUGHNESS_TEXTURES
#define MAX_ROUGHNESS_TEXTURES 1
#endif

#ifndef MAX_METALLIC_TEXTURES
#define MAX_METALLIC_TEXTURES 1
#endif

#ifndef MAX_AO_TEXTURES
#define MAX_AO_TEXTURES 1
#endif

#ifndef MAX_EMISSION_TEXTURES
#define MAX_EMISSION_TEXTURES 1
#endif

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D   diffuseMaps[MAX_DIFFUSE_TEXTURES];
    int         diffuseMapsCount;

    sampler2D   specularMaps[MAX_SPECULAR_TEXTURES];
    int         specularMapsCount;

    sampler2D   roughnessMaps[MAX_ROUGHNESS_TEXTURES];
    int         roughnessMapsCount;

    sampler2D   metallicMaps[MAX_METALLIC_TEXTURES];
    int         metallicMapsCount;

    sampler2D   aoMaps[MAX_AO_TEXTURES];
    int         aoMapsCount;

    sampler2D   emissionMaps[MAX_EMISSION_TEXTURES];
    int         emissionMapsCount;

    sampler2D   normalMap;
    bool        hasNormalMap;

    vec3        ambient;
    float       shininess;
	float       metallic;
    Attenuation emissionAttenuation;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform Material _Material;

void main()
{    
    FragColor = texture(_Material.roughnessMaps[0], TexCoords);
}