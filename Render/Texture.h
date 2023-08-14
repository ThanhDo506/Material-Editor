#ifndef TEXTURE_H
#define TEXTURE_H

#include <algorithm>
#include <glm/glm.hpp>
#include <string>
#include <gl/glew.h>
#include <vector>
#include "../third-party/stb/stb_image.h"

// The type of texture binding.
enum class TextureBindType {
    // By default we bind by texture type.
    BY_TEXTURE_TYPE = 0,
    // A TEXTURE_2D.
    TEXTURE_2D,
    // A cubemap.
    CUBEMAP,
    // An image texture that is directly indexed, rather than sampled.
    IMAGE_TEXTURE,
};


enum class TextureType {
    TEXTURE_2D = 0,
    CUBEMAP
};

inline const TextureBindType textureTypeToTextureBindType(TextureType type) {
    switch (type) {
    case TextureType::TEXTURE_2D:
        return TextureBindType::TEXTURE_2D;
    case TextureType::CUBEMAP:
        return TextureBindType::CUBEMAP;
    }
    throw std::string("ERROR::TEXTURE::INVALID_TEXTURE_TYPE\n" +
                           std::to_string(static_cast<int>(type)));
}

inline const GLenum textureTypeToGlTarget(TextureType type) {
    switch (type) {
    case TextureType::TEXTURE_2D:
        return GL_TEXTURE_2D;
    case TextureType::CUBEMAP:
        return GL_TEXTURE_CUBE_MAP;
    }
    throw "ERROR::TEXTURE::INVALID_TEXTURE_TYPE::" + std::to_string(static_cast<int>(type));
}

enum class TextureFiltering {
    // Uses nearest-neighbor sampling.
    NEAREST = 0,
    // Uses linear interpolation between texels.
    BILINEAR,
    // Uses linear interpolation between mipmap levels and their texels.
    TRILINEAR,
    // Handles anisotropy when sampling.
    ANISOTROPIC,
};

enum class TextureWrapMode {
    REPEAT = 0,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
};

enum class MipGeneration {
    // Never generates or reserves mipmaps.
    NEVER = 0,
    // Generates mipmaps when loading, but not when creating empty textures.
    ON_LOAD,
    // Always attempts to generate mipmaps when possible.
    ALWAYS,
};

struct TextureParams {
    bool flipVerticallyOnLoad = true;
    TextureFiltering filtering = TextureFiltering::NEAREST;
    TextureWrapMode wrapMode = TextureWrapMode::REPEAT;
    glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    MipGeneration generateMips = MipGeneration::ON_LOAD;
    int maxNumMips = -1;
};

inline int calculateNumMips(int width, int height) {
    return 1 + static_cast<int>(std::floor(std::log2(std::max(width, height))));
}

inline glm::vec2 calculateNextMip(const glm::vec2& mipSize) {
    return glm::vec2 (std::max(mipSize.x / 2, 1.0f), std::max(mipSize.y / 2, 1.0f));
}

inline glm::vec2 calculateMipLevel(int mip0Width, int mip0Height, int level) {
    glm::vec2 size = {mip0Width, mip0Height};
    if (level == 0) {
        return size;
    }
    for (int mip = 0; mip < level; ++mip) {
        size = calculateNextMip(size);
    }
    return size;
}

class Texture
{
public:
    ~Texture();
    // Loads a texture from a given path.
    static Texture load(const char* path, bool isSRGB = true);
    static Texture load(const char* path, bool isSRGB, const TextureParams& params);

    // Loads an HDR texture from the given path.
    static Texture loadHdr(const char* path);
    static Texture loadHdr(const char* path, const TextureParams& params);
    
    static Texture loadCubemap(std::vector<std::string> faces);
    static Texture loadCubemap(std::vector<std::string> faces, const TextureParams& params);
    // Creates a custom texture of the given size and format.
    static Texture create(int width, int height, GLenum internalFormat);
    static Texture create(int width, int height, GLenum internalFormat, const TextureParams& params);
    static Texture createCubemap(int size, GLenum internalFormat);
    static Texture createCubemap(int size, GLenum internalFormat, const TextureParams& params);

    // Delete the texture
    void clean();

    void bindToUnit(unsigned int textureUnit, TextureBindType bindType = TextureBindType::BY_TEXTURE_TYPE);

    void generateMips(int maxNumMips = -1);

    void setSamplerMipRange(int min, int max);
    void unsetSamplerMipRange();
    
    GLuint getId() const { return _id; }
    std::string getPath() const { return _path; }
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    int getNumChannels() const { return _numChannels; }
    int getNumMips() const { return _numMips; }
    GLenum getInternalFormat() const { return _internalFormat; }
private:
    GLuint _id;
    std::string _path;
    int _width;
    int _height;
    int _numChannels;
    int _numMips;
    GLenum _internalFormat;
    TextureType _type;

    static void applyParams(const TextureParams& params, TextureType type = TextureType::TEXTURE_2D);

    friend class Model;
    friend class Mesh;
    friend class GUI;
};

#endif
