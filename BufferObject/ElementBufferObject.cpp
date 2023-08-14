#include "ElementBufferObject.h"

ElementBufferObject::~ElementBufferObject()
{
    glDeleteBuffers(1, &_id);
}

ElementBufferObject::ElementBufferObject(std::vector<GLuint>& indices)
{
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

GLuint ElementBufferObject::setBuffer(std::vector<GLuint>& indices)
{
    if(_id)
        glDeleteBuffers(1, &_id);
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    return _id;
}

void ElementBufferObject::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void ElementBufferObject::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::Delete()
{
    glDeleteBuffers(1, &_id);
}
