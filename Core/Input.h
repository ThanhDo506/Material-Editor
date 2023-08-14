#pragma once
#include <glm/vec2.hpp>
#include <glfw/glfw3.h>

class Input
{
public:
    static Input& instance();
    static glm::vec2 getMousePosition();
    static glm::vec2 getDeltaMousePosition();

    static void setMousePosition(glm::vec2 pos);
    static void setDeltaMousePosition(glm::vec2 pos);
    GLFWwindow * getWindowManipulator() const;
private:
    Input(const Input& obj)           = delete;
    void operator=(const Input& obj)  = delete;
    Input();
    

    void reset();
    
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    
    GLFWwindow * p_glfwWindow;
    glm::vec2 _mousePosition = {0.0,0.0};
    glm::vec2 _lastMousePosition = {0.0,0.0};
    glm::vec2 _deltaMousePosition = {0.0,0.0};
    friend class Application;
};
