#pragma once

#include <vector>
#include <gl/glew.h>
#include "../World Entities/Vertex.h"

class VertexBufferObject
{
public:
    VertexBufferObject() = default;
    ~VertexBufferObject();
    template <typename T>
    VertexBufferObject(std::vector<T>& buffer);

    template <typename T>
    GLuint setBuffer(std::vector<T>& buffer);
    
    void Bind();
    void Unbind();
    void Delete();

    GLuint _id;
};
