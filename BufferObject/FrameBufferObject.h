#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

#include <gl/glew.h>

class FrameBufferObject
{
public:
    FrameBufferObject();

    void Bind();
    void Unbind();
    void Delete();

private:
    GLuint _id;
};


#endif