#include "VertexAttributeObject.h"

VertexAttributeObject::VertexAttributeObject()
{
    glGenVertexArrays(1, &_id);
}

VertexAttributeObject::~VertexAttributeObject()
{
    glDeleteVertexArrays(1, &_id);
}

void VertexAttributeObject::Bind()
{
    glBindVertexArray(_id);
}

void VertexAttributeObject::Unbind()
{
    glBindVertexArray(0);
}

void VertexAttributeObject::Delete()
{
    glDeleteVertexArrays(1, &_id);
}

void VertexAttributeObject::linkAttrib(VertexBufferObject& vbo, GLuint   layout, GLint numComponents, GLenum type,
                                       GLsizei          stride, void* offset)
{
    vbo.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.Unbind();
}

void VertexAttributeObject::linkAttrib(GLuint     vboId, GLuint layout, GLint numComponents, GLenum type,
                                       GLsizei stride, void* offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
