#include "Camera.h"

Camera::Camera(Transform transform, int screenWidth, int screenHeight, float near, float far, float zoom)
{
    this->transform  = transform;
    this->_width     = screenWidth;
    this->_height    = screenHeight;
    this->_near      = near;
    this->_far       = far;
    this->_zoom      = zoom;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(transform.position, transform.position + transform.forward(), transform.up());
}

glm::mat4 Camera::getPerspectiveProjectionMatrix() const
{
    return glm::perspective(glm::radians(_zoom), _width * 1.0f/ _height , _near, _far);
}

glm::mat4 Camera::getOrthoProjectionMatrix() const
{
    return glm::ortho(0.0f, _width * 1.0f, 0.0f, _height * 1.0f, _near, _far);
}
