#ifndef GUI_H
#define GUI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../World Entities/WorldManager.h"
#include "../../Core/Application.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#pragma region ImGui lib
#include "../../third-party/imgui/imgui.h"
#include "../../third-party/imgui/imgui_impl_glfw.h"
#include "../../third-party/imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#pragma endregion

#include "../TextureMap.h"

class GUI
{
    friend class Application;

public:
    static GUI& instance();

    void init(Application* application, const char* glfwVersion = "#version 330");
    void draw();
    void clean();
private:
    bool         show_demo_window = false;
    Application* p_application;
    GUI();
    GUI(const GUI& obj)     = delete;
    void operator=(const GUI& obj) = delete;

    void textureModelHandle(TextureMap* texture);
};

#endif