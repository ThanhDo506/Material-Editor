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
    glm::vec3               _ambient;
    Light::Attenuation      _emissionAttenuation;
    float                   _shininess;
    float                   _metallic;
    Material();
};


#endif