#include "Input.h"

Input& input = Input::instance();


Input::Input()
{
    p_glfwWindow = nullptr;
}

void Input::init(GLFWwindow * p_glfwWindow)
{
    this->p_glfwWindow = p_glfwWindow;
}

void Input::update()
{
    if (glfwGetKey(p_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(p_glfwWindow, 1);
    } else if (glfwGetKey(p_glfwWindow, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (glfwGetKey(p_glfwWindow, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (glfwGetKey(p_glfwWindow, GLFW_KEY_3) == GLFW_PRESS)
    {
        glPointSize(10.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    } else if (glfwGetKey(p_glfwWindow, GLFW_KEY_4) == GLFW_PRESS)
    {
        glPointSize(1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
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
