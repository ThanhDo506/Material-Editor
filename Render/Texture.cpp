#include "Texture.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#define MAX_ANISOTROPY_SAMPLES 4.0f

Texture Texture::load(const char* path, bool isSRGB)
{
    TextureParams params = {
        false,
        true,
        TextureFiltering::ANISOTROPIC,
        TextureWrapMode::REPEAT,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        MipGeneration::ON_LOAD,
        -1,
    };
    return load(path, isSRGB, params);
}

Texture Texture::load(const char* path, bool isSRGB, const TextureParams& params)
{
    Texture texture;
    texture._type = TextureType::TEXTURE_2D;

    stbi_set_flip_vertically_on_load(params.flipVerticallyOnLoad);
    unsigned char* data =
        stbi_load(path, &texture._width, &texture._height, &texture._numChannels,0);

    if (data == nullptr)
    {
        stbi_image_free(data);
        throw std::string("ERROR::TEXTURE::LOAD_FAILED\n" + std::string(path));
    }

    GLenum dataFormat;
    if (texture._numChannels == 1)
    {
        texture._internalFormat = GL_R8;
        dataFormat = GL_RED;
    }
    else if (texture._numChannels == 2)
    {
        texture._internalFormat = GL_RG8;
        dataFormat = GL_RG;
    }
    else if (texture._numChannels == 3)
    {
        texture._internalFormat = isSRGB ? GL_SRGB8 : GL_RGB8;
        dataFormat = GL_RGB;
    }
    else if (texture._numChannels == 4)
    {
        texture._internalFormat = isSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else
    {
        stbi_image_free(data);
        throw std::string(
            "ERROR::TEXTURE::UNSUPPORTED_TEXTURE_FORMAT\n"
            "Texture '" +
            std::string(path) + "' contained unsupported number of channels: " +
            std::to_string(texture._numChannels));
    }

    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_2D, texture._id);

    glTexImage2D(GL_TEXTURE_2D, 0, texture._internalFormat,
                 texture._width, texture._height, 0,
                 dataFormat, GL_UNSIGNED_BYTE, data);
    if (params.generateMips >= MipGeneration::ON_LOAD)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        texture._numMips = calculateNumMips(texture._width, texture._height);
    }
    else
    {
        texture._numMips = 1;
    }

    // Set texture-wrapping/filtering options.
    applyParams(params, texture._type);

    stbi_image_free(data);

    return texture;
}

Texture Texture::loadHdr(const char* path)
{
    TextureParams params = {
        false,
        true,
        TextureFiltering::BILINEAR,
        TextureWrapMode::CLAMP_TO_EDGE,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        MipGeneration::ON_LOAD,
        -1,
    };
    return loadHdr(path, params);
}

Texture Texture::loadHdr(const char* path, const TextureParams& params)
{
    Texture texture;
    texture._type = TextureType::TEXTURE_2D;
    texture._numMips = 1;

    stbi_set_flip_vertically_on_load(params.flipVerticallyOnLoad);
    float* data = stbi_loadf(path, &texture._width, &texture._height,
                             &texture._numChannels, 0);

    if (data == nullptr)
    {
        stbi_image_free(data);
        throw std::string("ERROR::TEXTURE::LOAD_FAILED\n" + std::string(path));
    }

    GLenum dataFormat;
    if (texture._numChannels == 1)
    {
        texture._internalFormat = GL_R16F;
        dataFormat = GL_RED;
    }
    else if (texture._numChannels == 2)
    {
        texture._internalFormat = GL_RG16F;
        dataFormat = GL_RG;
    }
    else if (texture._numChannels == 3)
    {
        texture._internalFormat = GL_RGB16F;
        dataFormat = GL_RGB;
    }
    else if (texture._numChannels == 4)
    {
        texture._internalFormat = GL_RGBA16F;
        dataFormat = GL_RGBA;
    }
    else
    {
        stbi_image_free(data);
        throw std::string(
            "ERROR::TEXTURE::UNSUPPORTED_TEXTURE_FORMAT\n"
            "Texture '" +
            std::string(path) + "' contained unsupported number of channels: " +
            std::to_string(texture._numChannels));
    }

    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_2D, texture._id);

    glTexImage2D(GL_TEXTURE_2D, 0, texture._internalFormat,
                 texture._width, texture._height, 0,
                 dataFormat, GL_FLOAT, data);

    // Set texture-wrapping/filtering options.
    applyParams(params, texture._type);

    stbi_image_free(data);

    return texture;
}

Texture Texture::loadCubemap(std::vector<std::string> faces)
{
    TextureParams params = {
        false,
        true,
        TextureFiltering::BILINEAR,
        TextureWrapMode::CLAMP_TO_EDGE,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        MipGeneration::ON_LOAD,
        -1,
    };
    return loadCubemap(faces, params);
}

Texture Texture::loadCubemap(std::vector<std::string> faces,
                             const TextureParams& params)
{
    stbi_set_flip_vertically_on_load(params.flipVerticallyOnLoad);
    if (faces.size() != 6)
    {
        throw std::string(
            "ERROR::TEXTURE::INVALID_ARGUMENT\nMust pass exactly 6 faces to "
            "loadCubemap");
    }

    Texture texture;
    texture._type = TextureType::CUBEMAP;
    texture._numMips = 1;
    texture._internalFormat = GL_RGB8; // Cubemaps must be RGB.

    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture._id);

    int width, height, numChannels;
    bool initialized = false;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height,
                                        &numChannels, /*desired_channels=*/0);
        // Error handling.
        if (data == nullptr)
        {
            stbi_image_free(data);
            throw std::string("ERROR::TEXTURE::LOAD_FAILED\n" + faces[i]);
        }
        if (numChannels != 3)
        {
            stbi_image_free(data);
            throw std::string(
                "ERROR::TEXTURE::UNSUPPORTED_TEXTURE_FORMAT\n"
                "Cubemap texture '" +
                faces[i] + "' contained unsupported number of channels: " +
                std::to_string(numChannels));
        }
        if (!initialized)
        {
            if (width != height)
            {
                throw std::string(
                    "ERROR::TEXTURE::INVALID_TEXTURE_SIZE\n"
                    "Cubemap texture '" +
                    faces[i] + "' was not square");
            }
            texture._width = width;
            texture._height = height;
            texture._numChannels = numChannels;
        }
        else if (width != texture._width || height != texture._height)
        {
            throw std::string(
                "ERROR::TEXTURE::INVALID_TEXTURE_SIZE\n"
                "Cubemap texture '" +
                faces[i] + "' was a different size than the first face");
        }

        // Load into the next cube map texture position.
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     texture._internalFormat, width, height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    applyParams(params, texture._type);

    return texture;
}

Texture Texture::create(int width, int height, GLenum internalFormat)
{
    TextureParams params = {
        false,
        true,
        TextureFiltering::BILINEAR,
        TextureWrapMode::CLAMP_TO_EDGE,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        MipGeneration::ON_LOAD,
        -1,
    };
    return create(width, height, internalFormat, params);
}

Texture Texture::create(int width, int height, GLenum internalFormat,
                        const TextureParams& params)
{
    Texture texture;
    texture._type = TextureType::TEXTURE_2D;
    texture._width = width;
    texture._height = height;
    texture._numChannels = 0; // Default.
    texture._numMips = 1;
    if (params.generateMips == MipGeneration::ALWAYS)
    {
        texture._numMips = calculateNumMips(texture._width, texture._height);
        if (params.maxNumMips >= 0)
        {
            texture._numMips = std::min(texture._numMips, params.maxNumMips);
        }
    }
    texture._internalFormat = internalFormat;

    glGenTextures(1, &texture._id );
    glBindTexture(GL_TEXTURE_2D, texture._id);

    glTexStorage2D(GL_TEXTURE_2D, texture._numMips, texture._internalFormat,
                   texture._width, texture._height);

    // Set texture-wrapping/filtering options.
    applyParams(params, texture._type);

    return texture;
}

Texture Texture::createCubemap(int size, GLenum internalFormat)
{
    TextureParams params = {
        false,
        true,
        TextureFiltering::BILINEAR,
        TextureWrapMode::CLAMP_TO_EDGE,
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        MipGeneration::ON_LOAD,
        -1,
    };
    return createCubemap(size, internalFormat, params);
}

Texture Texture::createCubemap(int size, GLenum internalFormat,
                               const TextureParams& params)
{
    Texture texture;
    texture._type = TextureType::CUBEMAP;
    texture._width = size;
    texture._height = size;
    texture._numChannels = 0; // Default.
    texture._numMips = 1;
    if (params.generateMips == MipGeneration::ALWAYS)
    {
        texture._numMips = calculateNumMips(texture._width, texture._height);
        if (params.maxNumMips >= 0)
        {
            texture._numMips = std::min(texture._numMips, params.maxNumMips);
        }
    }
    texture._internalFormat = internalFormat;

    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture._id);

    glTexStorage2D(GL_TEXTURE_CUBE_MAP, texture._numMips, texture._internalFormat,
                   texture._width, texture._height);

    applyParams(params, texture._type);

    return texture;
}

void Texture::bindToUnit(unsigned int textureUnit, TextureBindType bindType)
{
    // TODO: Take into account GL_MAX_TEXTURE_UNITS here.
    glActiveTexture(GL_TEXTURE0 + textureUnit);

    if (bindType == TextureBindType::BY_TEXTURE_TYPE)
    {
        bindType = textureTypeToTextureBindType(_type);
    }

    switch (bindType)
    {
    case TextureBindType::TEXTURE_2D:
        glBindTexture(GL_TEXTURE_2D, _id);
        break;
    case TextureBindType::CUBEMAP:
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
        break;
    case TextureBindType::IMAGE_TEXTURE:
        // Bind image unit.
        glBindImageTexture(textureUnit, _id, 0, GL_FALSE, 0,
                           GL_READ_WRITE, _internalFormat);
        break;
    default:
        throw std::string("ERROR::TEXTURE::INVALID_TEXTURE_BIND_TYPE\n" +
            std::to_string(static_cast<int>(bindType)));
    }
}

void Texture::setSamplerMipRange(int min, int max)
{
    GLenum target = textureTypeToGlTarget(_type);
    glBindTexture(target, _id);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, min);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, max);
}

void Texture::unsetSamplerMipRange()
{
    // OpenGL defaults.
    setSamplerMipRange(0, 1000);
}

void Texture::clean() { glDeleteTextures(1, &_id); }

void Texture::generateMips(int maxNumMips)
{
    if (maxNumMips >= 0)
    {
        setSamplerMipRange(0, maxNumMips);
    }
    GLenum type = textureTypeToGlTarget(_type);

    glBindTexture(type, _id);
    glGenerateMipmap(type);

    if (maxNumMips >= 0)
    {
        unsetSamplerMipRange();
    }
}

void Texture::applyParams(const TextureParams& params, TextureType type)
{
    GLenum target = textureTypeToGlTarget(type);
    switch (params.filtering)
    {
    case TextureFiltering::NEAREST:
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case TextureFiltering::BILINEAR:
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case TextureFiltering::TRILINEAR:
    case TextureFiltering::ANISOTROPIC:
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (params.filtering == TextureFiltering::ANISOTROPIC)
        {
            glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY,
                            MAX_ANISOTROPY_SAMPLES);
        }
        break;
    }

    switch (params.wrapMode)
    {
    case TextureWrapMode::REPEAT:
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if (type == TextureType::CUBEMAP)
        {
            glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
        }
        break;
    case TextureWrapMode::CLAMP_TO_EDGE:
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if (type == TextureType::CUBEMAP)
        {
            glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
        break;
    case TextureWrapMode::CLAMP_TO_BORDER:
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        if (type == TextureType::CUBEMAP)
        {
            glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        }
        glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR,
                         glm::value_ptr(params.borderColor));
        break;
    }
}
