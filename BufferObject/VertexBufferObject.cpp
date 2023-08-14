#include "VertexBufferObject.h"

template VertexBufferObject::VertexBufferObject(std::vector<Vertex>& buffer);
template VertexBufferObject::VertexBufferObject(std::vector<float>& buffer);

template <typename T>
VertexBufferObject::VertexBufferObject(std::vector<T>& buffer)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);
}

template GLuint VertexBufferObject::setBuffer(std::vector<Vertex>& buffer);
template GLuint VertexBufferObject::setBuffer(std::vector<float>& buffer);

template <typename T>
GLuint VertexBufferObject::setBuffer(std::vector<T>& buffer)
{
    if(_id)
        glDeleteBuffers(1, &_id);
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);
    return _id;
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &_id);
}

void VertexBufferObject::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBufferObject::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Delete()
{
    glDeleteBuffers(1, &_id);
}
