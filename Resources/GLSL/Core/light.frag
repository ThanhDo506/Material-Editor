#pragma once
#include "normal.frag"

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

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
    int         emissionMapsCOunt;

    sampler2D   normalMap;
    bool        hasNormalMap;

    vec3        ambient;
    float       shininess;
    Attenuation emissionAttenuation;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3        position;
    vec3        ambient;
    vec3        diffuse;
    vec3        specular;
    Attenuation attenuation;
};

struct SpotLight {
    vec3    position;
    vec3    direction;
    float   innerAngle;
    float   outerAngle;

    vec3    ambient;
    vec3    diffuse;
    vec3    specular;

    Attenuation attenuation;
};

float calculateAttenuation(Attenuation attenuation, float distance) {
    // Avoid dividing by zero.
    return 1.0 / max(attenuation.constant + attenuation.linear * distance 
    + attenuation.quadratic * (distance * distance), 1e-4);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection, Material material) {
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3    reflectDir  = reflect(-lightDir, normal);
    float   spec        = pow(max(dot(cameraDirection, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragmentPosition);
    float attenuation = calculateAttenuation(light.attenuation, distance);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMaps[0], TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMaps[0], TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMaps[0], TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 cameraDirection, Material material) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(cameraDirection, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMaps[0], TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMaps[0], TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMaps[0], TexCoords));
    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection, Material material) {
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(cameraDirection, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = calculateAttenuation(light.attenuation, cameraDirection);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerAngle - light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMaps[0], TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMaps[0], TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMaps[0], TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

// Calculate Intensity of SpotLight
float calculateSpotLightIntensity(SpotLight light, vec3 lightDir) {
    float theta = dot(lightDir, normalize(-light.direction));

    float innerAngleCosine = cos(light.innerAngle);
    float outerAngleCosine = cos(light.outerAngle);
    float epsilon = innerAngleCosine - outerAngleCosine;
    return clamp((theta - outerAngleCosine) / epsilon, 0.0, 1.0);
}

/** ============================ Ambient ============================ **/

/** Calculate the ambient shading component. */
vec3 shadeAmbientDeferred(vec3 albedo, vec3 ambient, float ao) {
    return albedo * ambient * ao;
}

/** Calculate shading for ambient component based on the given material. */
vec3 shadeAmbient(Material material, vec3 albedo, vec2 texCoords) {
    float ao = texture(material.aoMaps[0], texCoords).r;
    return shadeAmbientDeferred(albedo, material.ambient, ao);
}

/** ============================ Emission ============================ **/

/** Calculate deferred shading for emission based on an emission color. */
vec3 shadeEmissionDeferred(vec3 emissionColor, vec3 fragPos_viewSpace,
Attenuation emissionAttenuation) {
    // Calculate emission attenuation towards camera.
    float fragDist = length(fragPos_viewSpace);
    float attenuation = calculateAttenuation(emissionAttenuation, fragDist);
    // Emission component.
    return emissionColor * attenuation;
}

/** Calculate shading for emission textures on the given material. */
vec3 shadeEmission(Material material, vec3 fragPos_viewSpace, vec2 texCoords) {
    vec3 emission = texture(material.emissionMaps[0], texCoords).rgb;
    return shadeEmissionDeferred(emission, fragPos_viewSpace,
    material.emissionAttenuation);
}

/** ============================ Normals ============================ **/

/**
 * Looks up a normal from the material, using the provided TBN matrix to
 * convert from tangent space to the target space, or returns a vertex normal
 * if no normal map is present.
 */
vec3 getNormal(Material material, vec2 texCoords, mat3 TBN, vec3 vertexNormal) {
    if (material.hasNormalMap) {
        return normalize(TBN * sampleNormalMap(material.normalMap, texCoords));
    } else {
        return normalize(vertexNormal);
    }
}

/** ============================ Shadows ============================ **/

/** Calculate a shadow bias based on the surface normal and light direction. */
float shadowBias(float minBias, float maxBias, vec3 normal, vec3 lightDir) {
    return max(maxBias * (1.0 - dot(normal, lightDir)), minBias);
}

/** Sample from a shadow map using 9-texel percentage-closer filtering. */
float shadowSamplePCF(sampler2D shadowMap, vec2 shadowTexCoords, float currentDepth, float bias) {
    float shadow = 0.0;
    vec2 texelOffset = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth =
            texture(shadowMap, shadowTexCoords + vec2(x, y) * texelOffset).r;
            // Check whether in shadow.
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}

/**
 * Calculate whether the given fragment is in shadow.
 * Returns 1.0 if in shadow, 0.0 if not.
 */
float shadow(sampler2D shadowMap, vec4 fragPosLightSpace, float bias) {
    // Perform perspective divide.
    vec3 projectedPos = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Shift to the range 0..1 so that we can compare with depth.
    projectedPos = projectedPos * 0.5 + 0.5;
    // Check for out-of-frustum.
    if (projectedPos.z > 1.0) {
        // Assume not in shadow.
        return 0.0;
    }
    vec2 shadowTexCoords = projectedPos.xy;
    float currentDepth = projectedPos.z;
    return shadowSamplePCF(shadowMap, shadowTexCoords, currentDepth, bias);
}