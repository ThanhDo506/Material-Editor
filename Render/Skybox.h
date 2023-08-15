#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "../BufferObject/VertexAttributeObject.h"
#include "../BufferObject/VertexBufferObject.h"
#include "../BufferObject/ElementBufferObject.h"
#include "../World Entities/Camera.h"

class Skybox
{
public:
    enum class Face
    {
        RIGHT,
        LEFT,
        TOP,
        BOTTOM,
        FRONT,
        BACK
    };
    Skybox(std::vector<std::string> imagesPaths, const char* vertPath, const char* fragPath);
    Skybox(std::string const &imagesPaths, const char* vertPath, const char* fragPath);

    void draw(Camera& camera);
    void clean();
private:
    VertexAttributeObject    _vao;
    ElementBufferObject      _ebo;
    VertexBufferObject       _vbo;
    Texture                  _texture;
    Shader*                  _shader;

    // i should make these are constexpr or static
    std::vector<float> vertexPos = {
        //   Coordinates
        -1.0f, -1.0f, 1.0f,  //        7--------6
        1.0f, -1.0f, 1.0f,   //       /|       /|
        1.0f, -1.0f, -1.0f,  //      4--------5 |
        -1.0f, -1.0f, -1.0f, //      | |      | |
        -1.0f, 1.0f, 1.0f,   //      | 3------|-2
        1.0f, 1.0f, 1.0f,    //      |/       |/
        1.0f, 1.0f, -1.0f,   //      0--------1
        -1.0f, 1.0f, -1.0f
    };
    std::vector<GLuint> indices = {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
    };
};
#endif