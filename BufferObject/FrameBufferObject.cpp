#include "FrameBufferObject.h"

FrameBufferObject::FrameBufferObject()
{
    glGenFramebuffers(1, &_id);
}

void FrameBufferObject::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void FrameBufferObject::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::Delete()
{
    glDeleteFramebuffers(1, &_id);
}
