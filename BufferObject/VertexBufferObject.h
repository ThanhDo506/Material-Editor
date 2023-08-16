#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include <vector>
#include <gl/glew.h>
#include "../World Entities/Vertex.h"

class VertexBufferObject
{
public:
    VertexBufferObject() = default;
    template <typename T>
    VertexBufferObject(std::vector<T>& buffer);

    template <typename T>
    GLuint setBuffer(std::vector<T>& buffer);
    
    void Bind();
    void Unbind();
    void Delete();

    GLuint _id;
};
#endif
