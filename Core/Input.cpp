#include "Input.h"

Input& input = Input::instance();


Input::Input()
{
}

void Input::reset()
{
    Input::instance()._deltaMousePosition = Input::instance()._mousePosition - Input::instance()._lastMousePosition;
    _lastMousePosition = _mousePosition;
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void Input::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Input::instance()._mousePosition = glm::vec2(xposIn, yposIn);
    Input::instance()._deltaMousePosition = Input::instance()._mousePosition - Input::instance()._lastMousePosition;
}

Input& Input::instance()
{
    static Input instance;
    return instance;
}

glm::vec2 Input::getMousePosition()
{
    return Input::instance()._mousePosition;
}

glm::vec2 Input::getDeltaMousePosition()
{
    return Input::instance()._deltaMousePosition;
}

void Input::setMousePosition(glm::vec2 pos)
{
    Input::instance()._mousePosition = pos;
}

void Input::setDeltaMousePosition(glm::vec2 pos)
{
    Input::instance()._deltaMousePosition = pos;
}

GLFWwindow* Input::getWindowManipulator() const
{
    return p_glfwWindow;
}
