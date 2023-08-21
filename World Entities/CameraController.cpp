#include "CameraController.h"

#include <iostream>

CameraController::CameraController(Camera *camera, float speed,
                                   float sensitivityX,
                                   float sensitivityY)
{
    this->p_camera      = camera;
    this->_speed        = speed;
    this->_sensitivityX = sensitivityX;
    this->_sensitivityY = sensitivityY;
    glm::vec3 rotation = p_camera->transform.eulerAngles();
    rotationX = rotation.x;
    rotationY = rotation.y;
}

void CameraController::cameraMovement()
{
    // Handles key inputs
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_W) == GLFW_PRESS)
    {
        p_camera->transform.position += _speed * p_camera->transform.forward() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_A) == GLFW_PRESS)
    {
        p_camera->transform.position += _speed * p_camera->transform.right() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_S) == GLFW_PRESS)
    {
        p_camera->transform.position += _speed * -p_camera->transform.forward() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_D) == GLFW_PRESS)
    {
        p_camera->transform.position += _speed * -p_camera->transform.right() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        p_camera->transform.position += _speed * p_camera->transform.up() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        p_camera->transform.position += _speed * -p_camera->transform.up() * static_cast<float>(Time::instance().deltaTime());
    }
}

void CameraController::cameraRotation()
{
}

void CameraController::update()
{
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_F1) == GLFW_PRESS)
    {
        _isForcus = !_isForcus;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_T) == GLFW_PRESS)
    {
        rotationY += Time::instance().deltaTime() * _sensitivityY;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotationY -= Time::instance().deltaTime() * _sensitivityY;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_V) == GLFW_PRESS)
    {
        rotationX += Time::instance().deltaTime() * _sensitivityX;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_B) == GLFW_PRESS)
    {
        rotationX -= Time::instance().deltaTime() * _sensitivityX;
    }

    if (rotationX <= -89.5f)
    {
        rotationX = -89.5;
    } else if (rotationX >= 89.5f)
    {
        rotationX = 89.5f;
    }

    p_camera->transform.rotation = Transform::eulerAnglesToQuaternion(rotationX, rotationY, 0);



    

    if (_isForcus)
    {
        this->cameraRotation();
    }
    cameraMovement();
}
