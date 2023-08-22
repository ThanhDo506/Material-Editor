#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include "Texture.h"
#include <iostream>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
enum class TextureMapType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    AO,
    EMISSION,
    ROUGHNESS,
    METALLIC,
    CUBEMAP,
    NONE
};

inline std::vector<aiTextureType> textureMapTypeToAiTextureTypes(
    TextureMapType type) {
    switch (type) {
    case TextureMapType::DIFFUSE:
        return {aiTextureType_DIFFUSE};
    case TextureMapType::SPECULAR:
        // Use metalness for specularity as well. When this is loaded as a
        // combined "metalnessRoughnessTexture", shaders should read the blue
        // channel.
        return {aiTextureType_SPECULAR, aiTextureType_METALNESS};
    case TextureMapType::ROUGHNESS:
        return {aiTextureType_DIFFUSE_ROUGHNESS};
    case TextureMapType::METALLIC:
        return {aiTextureType_METALNESS};
    case TextureMapType::AO:
        // For whatever reason, assimp stores AO maps as "lightmaps", even though
        // there's aiTextureType_AMBIENT_OCCLUSION...
        return {aiTextureType_LIGHTMAP};
    case TextureMapType::EMISSION:
        return {aiTextureType_EMISSIVE};
    case TextureMapType::NORMAL:
        return {aiTextureType_NORMALS};
    default:
        throw "ERROR::TEXTURE_MAP::INVALID_TEXTURE_MAP_TYPE\n" + std::to_string(static_cast<int>(type));
    }
}

class TextureMap
{
public:
    TextureMap(const Texture& texture, TextureMapType type = TextureMapType::NONE)
        : _texture(texture), _type(type) { _initialized = true; }
    TextureMap() { _type = TextureMapType::NONE; _initialized = false; }
    
    Texture& getTexture() { return _texture; }
    TextureMapType getType() const { return _type; }

    void clean();

    static TextureMap defaultTextureMap();
    static std::string to_string(TextureMapType type);
private:
    TextureMapType _type;
    Texture _texture;
    bool _initialized = false;

    friend class Model;
    friend class Mesh;
    friend class GUI;
    friend class Material;
};

#endif
