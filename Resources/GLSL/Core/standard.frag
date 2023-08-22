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

    vec4        ambient;
    float       shininess;
	float       metallic;
    float       normalMultiplier;
    bool        emissionOn;
    Attenuation emissionAttenuation;
};

#ifndef MAX_DIRECTIONAL_LIGHT
#define MAX_DIRECTIONAL_LIGHT 10
#endif

#ifndef MAX_POINT_LIGHT
#define MAX_POINT_LIGHT 10
#endif

#ifndef MAX_SPOT_LIGHT
#define MAX_SPOT_LIGHT 10
#endif

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

/************************** LIGHTING ************************/
float calculateAttenuation(Attenuation attenuation, float distance);

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection);

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 cameraDirection);

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection);
/***********************************************************/

/*************************** PBR ****************************/
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
/************************************************************/

out vec4 FragColor;

in VS_OUT {
	vec3 CameraPosition;
    vec3 FragmentPosition;
    vec3 Normal;
    vec4 Color;
    vec2 TexCoord;
    vec3 Tangent;
    mat3 TBN;
    vec3 TangentViewPosition;
    vec3 TangentFragPosition;
} fs_in;

uniform Material _Material;

// Lighting
uniform DirectionalLight _DirectionalLights[MAX_DIRECTIONAL_LIGHT];
uniform int _DirectionalLightCount;

uniform PointLight _PointLights[MAX_POINT_LIGHT];
uniform int _PointLightCount;

uniform SpotLight _SpotLights[MAX_SPOT_LIGHT];
uniform int _SpotLightCount;

uniform bool _UseBlinnPhong;
uniform bool _Gamma;

void main() {
	vec3 cameraDirection = normalize(fs_in.CameraPosition - fs_in.FragmentPosition);

	//*********** CALCULATE NORMAL ***********//
    vec3 normal = normalize(fs_in.Normal);
	if(_Material.hasNormalMap) {
		normal = texture(_Material.normalMap, fs_in.TexCoord).rgb * 2.0 - 1.0;
		normal = normalize(fs_in.TBN * normal);
	}
	//**************** PBR *****************//
	vec3 albedo     = pow(texture(_Material.diffuseMaps[0], fs_in.TexCoord).rgb, vec3(GAMMA));
    float metallic  = texture(_Material.metallicMaps[0], fs_in.TexCoord).r;
    float roughness = texture(_Material.roughnessMaps[0], fs_in.TexCoord).r;
    float ao        = texture(_Material.aoMaps[0], fs_in.TexCoord).r;
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	// reflectance equation
	vec3 Lo = vec3(0.0);
    // Calculate Directional light
    for(int i = 0; i < _DirectionalLightCount; i++) {
        // calculate per-light radiance
        vec3 L = normalize(_DirectionalLights[i].direction);
        vec3 H = normalize(cameraDirection + L);
        vec3 radiance = _PointLights[i].ambient;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, roughness);
        float G   = GeometrySmith(normal, cameraDirection, L, roughness);
        vec3  F   = fresnelSchlick(max(dot(H, cameraDirection), 0.0), F0);

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, cameraDirection), 0.0) * max(dot(normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(normal, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    
    // Calculate Point Light
	for(int i = 0; i < _PointLightCount; i++) {
		// calculate per-light radiance
        vec3 L = normalize(_PointLights[i].position - fs_in.FragmentPosition);
        vec3 H = normalize(cameraDirection + L);
        float distance = length(_PointLights[i].position - fs_in.FragmentPosition);
        float attenuation = calculateAttenuation(_PointLights[i].attenuation, distance);
        vec3 radiance = _PointLights[i].ambient * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, roughness);   
        float G   = GeometrySmith(normal, cameraDirection, L, roughness);      
        vec3  F   = fresnelSchlick(max(dot(H, cameraDirection), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(normal, cameraDirection), 0.0) * max(dot(normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(normal, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

	vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/GAMMA)); 
    //**************************************//

	FragColor = vec4(color, 1.0);
}

/*************************** LIGHTING ******************************/
float calculateAttenuation(Attenuation attenuation, float distance) {
    // Avoid dividing by zero.
    return 1.0 / max(attenuation.constant + attenuation.linear * distance
    + attenuation.quadratic * (distance * distance), 1e-4);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection) 
{
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(_UseBlinnPhong) {
        vec3 halfwayDir = normalize(lightDir + cameraDirection);
        spec = pow(max(dot(normal, halfwayDir), 0.0), _Material.shininess);
    } else {
        vec3 reflectDir  = reflect(-lightDir, normal);
        spec = pow(max(dot(cameraDirection, reflectDir), 0.0), _Material.shininess);
    }
    // attenuation
    float distance    = length(light.position - fragmentPosition);
    float attenuation = calculateAttenuation(light.attenuation, distance);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(_Material.diffuseMaps[0], fs_in.TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(_Material.diffuseMaps[0], fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(_Material.specularMaps[0], fs_in.TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 cameraDirection) 
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	float spec = 0;
    if(_UseBlinnPhong) {
        vec3 halfwayDir = normalize(lightDir + cameraDirection);
        spec = pow(max(dot(normal, halfwayDir), 0.0), _Material.shininess);
    } else {
        vec3 reflectDir  = reflect(-lightDir, normal);
        spec = pow(max(dot(cameraDirection, reflectDir), 0.0), _Material.shininess);
    }
    // combine results
    vec3 ambient = light.ambient * vec3(texture(_Material.diffuseMaps[0], fs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(_Material.diffuseMaps[0], fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(_Material.specularMaps[0], fs_in.TexCoord));
    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection) {
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	float spec = 0;
    if(_UseBlinnPhong) {
        vec3 halfwayDir = normalize(lightDir + cameraDirection);
        spec = pow(max(dot(normal, halfwayDir), 0.0), _Material.shininess);
    } else {
        vec3 reflectDir  = reflect(-lightDir, normal);
        spec = pow(max(dot(cameraDirection, reflectDir), 0.0), _Material.shininess);
    }
    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = calculateAttenuation(light.attenuation, distance);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerAngle - light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(_Material.diffuseMaps[0], fs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(_Material.diffuseMaps[0], fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(_Material.specularMaps[0], fs_in.TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
/*******************************************************************/

/**************************** PBR **********************************/
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
/*******************************************************************/