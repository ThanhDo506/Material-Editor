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
}

void CameraController::cameraMovement()
{
    // Handles key inputs
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_W) == GLFW_PRESS)
    {
        std::cout << "forward\n";
        p_camera->transform.position += _speed * p_camera->transform.forward() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_A) == GLFW_PRESS)
    {
        std::cout << "left\n";
        p_camera->transform.position += _speed * p_camera->transform.right() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_S) == GLFW_PRESS)
    {
        std::cout << "backward\n";
        p_camera->transform.position += _speed * -p_camera->transform.forward() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_D) == GLFW_PRESS)
    {
        std::cout << "right\n";
        p_camera->transform.position += _speed * -p_camera->transform.right() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        std::cout << "up\n";
        p_camera->transform.position += _speed * p_camera->transform.up() * static_cast<float>(Time::instance().deltaTime());
    }
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        std::cout << "down\n";
        p_camera->transform.position += _speed * -p_camera->transform.up() * static_cast<float>(Time::instance().deltaTime());
    }
}

void CameraController::cameraRotation()
{
}

void CameraController::update()
{
    glm::quat currentRotation = p_camera->transform.rotation;
    if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_F1) == GLFW_PRESS)
    {
        _isForcus = !_isForcus;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_T) == GLFW_PRESS)
    {
        p_camera->transform.rotation.y += Time::instance().deltaTime() * _sensitivityY / 100;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_Y) == GLFW_PRESS)
    {
        p_camera->transform.rotation.y -= Time::instance().deltaTime() * _sensitivityY / 100;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_V) == GLFW_PRESS)
    {
        p_camera->transform.rotation.x += Time::instance().deltaTime() * _sensitivityX / 100;
    } else if (glfwGetKey(Input::instance().getWindowManipulator(), GLFW_KEY_B) == GLFW_PRESS)
    {
        p_camera->transform.rotation.x -= Time::instance().deltaTime() * _sensitivityX / 100;
    }
    

    if (_isForcus)
    {
        this->cameraRotation();
    }
    cameraMovement();
}
