#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight, Transform transform, float near, float far, float zoom)
{
    this->transform  = transform;
    _width     = screenWidth;
    _height    = screenHeight;
    _near      = near;
    _far       = far;
    _zoom      = zoom;
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

void Camera::init()
{
}

void Camera::update()
{
}
