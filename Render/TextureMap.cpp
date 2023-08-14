#include "TextureMap.h"

void TextureMap::clean()
{
    _texture.clean();
}

std::string TextureMap::to_string(TextureMapType type)
{
    switch (type)
    {
    case TextureMapType::DIFFUSE:
        return "Diffuse";
    case TextureMapType::SPECULAR:
        return "Specular";
    case TextureMapType::NORMAL:
        return "Normal";
    case TextureMapType::AO:
        return "AO";
    case TextureMapType::EMISSION:
        return "Emission";
    case TextureMapType::ROUGHNESS:
        return "Roughness";
    case TextureMapType::METALLIC:
        return "Metallic";
    case TextureMapType::CUBEMAP:
        return "CubeMap";
    default:
        return "None";
    }
    return "None";
}
