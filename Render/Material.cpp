#include "Material.h"


Material::Material()
{
    TextureMap defaultDiffuse(Texture::load(defaultImage.c_str()));
    _diffuseMaps.push_back(defaultDiffuse);
    _diffuseMaps[0]._type = TextureMapType::DIFFUSE;

    TextureMap defaultSpecular(Texture::load(defaultImage.c_str()));
    _specularMaps.push_back(defaultSpecular);
    _specularMaps[0]._type = TextureMapType::SPECULAR;

    TextureMap defaultRoughness(Texture::load(defaultImage.c_str()));
    _roughnessMaps.push_back(defaultRoughness);
    _roughnessMaps[0]._type = TextureMapType::ROUGHNESS;

    TextureMap defaultMetallic(Texture::load(defaultImage.c_str()));
    _metallicMaps.push_back(defaultMetallic);
    _metallicMaps[0]._type = TextureMapType::METALLIC;

    TextureMap defaultAO(Texture::load(defaultImage.c_str()));
    _aoMaps.push_back(defaultAO);
    _aoMaps[0]._type = TextureMapType::AO;

    TextureMap defaultEmission(Texture::load(defaultImage.c_str()));
    _emissionMaps.push_back(defaultEmission);
    _emissionMaps[0]._type = TextureMapType::EMISSION;

    TextureMap defaultNormal(Texture::load(defaultImage.c_str()));
    _normalMap = defaultNormal;
    _normalMap._type = TextureMapType::NORMAL;

}
