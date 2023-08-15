#pragma once
#include <vector>
#include <gl/glew.h>

class ElementBufferObject
{
public:
    ElementBufferObject() = default;
    ElementBufferObject(std::vector<GLuint>& indices);
    GLuint setBuffer(std::vector<GLuint>& indices);
    
    void Bind();
    void Unbind();
    void Delete();

    GLuint _id;
};
