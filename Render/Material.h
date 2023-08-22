#ifndef MATERIAL_H
#define MATERIAL_H

#include "TextureMap.h"
#include "Lighting/Light.h"

class Material
{
public:
//********************* I will do that later **********************//
    enum RenderFace
    {
        BOTH,
        FRONT,
        BACK
    };
    /**
     * \brief False is Specular, True is Metallic
     */
    bool                    _workFlowMode;

    /**
     * \brief False is Opaque, True is Transparent
     */
    bool                    _surfaceType;
    RenderFace              _renderFace;
    float                   _alphaClipping = 0.0;
//****************************************************************//
    
    std::vector<TextureMap> _diffuseMaps;
    std::vector<TextureMap> _specularMaps;
    std::vector<TextureMap> _roughnessMaps;
    std::vector<TextureMap> _metallicMaps;
    std::vector<TextureMap> _aoMaps;
    std::vector<TextureMap> _emissionMaps;
    TextureMap              _normalMap;
    glm::vec4               _albedo;
    Light::Attenuation      _emissionAttenuation;
    float                   _shininess = 16;
    float                   _metallic = 0.02f;
    float                   _normalMultiplier = 1.0f;
    float                   _roughnessMultiplier = 1.0f;
    glm::vec3               _f0 = glm::vec3(0.04f);
    bool                    _emissionOn = false;
    bool                    _useMetallicMap = false;
    Material();
};


#endif