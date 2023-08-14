#pragma once

#include <gl/glew.h>

#include "VertexBufferObject.h"

class VertexAttributeObject
{
public:
    VertexAttributeObject();
    ~VertexAttributeObject();
    void Bind();
    void Unbind();
    void Delete();
    void linkAttrib(VertexBufferObject& vbo, GLuint layout, GLint numComponents, GLenum type, GLsizei stride,
                    void*               offset);
    void linkAttrib(GLuint vboId, GLuint layout, GLint numComponents, GLenum type, GLsizei stride, void* offset);


    GLuint _id;
};
