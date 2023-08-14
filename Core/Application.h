#pragma once

#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application
{
public:
    Application(std::string const &windowName  = "GLFW window",
                int         const &sceneWidth  = 800,
                int         const &sceneHeight = 600,
                glm::vec4   viewPort    = glm::vec4(0, 0, 800, 600),
                glm::vec4   clearColor  = glm::vec4(0, 0, 0, 1));
    ~Application();
    void run();
private:
    GLFWwindow *p_glfwWindow;
    std::string _name;
    glm::vec4 _viewPort;
    glm::vec4 _clearColor;
    int _width, _height;
    bool _isVsync;

    static GLuint loadTexture(const char* path);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
