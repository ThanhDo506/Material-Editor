#include "Application.h"

#include <iostream>

#include "Input.h"
#include "Time.h"
#include "../World Entities/Camera.h"
#include "../World Entities/CameraController.h"
#include "../Render/Shader.h"
#include "../Render/GUI/GUI.h"
#include "../Render/Lighting/LightManager.h"

#include "../third-party/stb/stb_image.h"

#include "../World Entities/Vertex.h"
#include "../Render/Mesh.h"
#include "../Render/Model.h"
#include "../Render/Skybox.h"

unsigned int loadTexture(const char *path);

Application::Application(std::string const &windowName, int const &sceneWidth, int const &sceneHeight, glm::vec4 viewPort,
                         glm::vec4 clearColor)
{
    _name = windowName;
    _width = sceneWidth;
    _height = sceneHeight;
    _viewPort = viewPort;
    _clearColor = clearColor;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->p_glfwWindow = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(p_glfwWindow);

    glfwSetFramebufferSizeCallback(p_glfwWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(p_glfwWindow, Input::mouse_callback);
    glfwSetScrollCallback(p_glfwWindow, Input::scroll_callback);
    
    glfwSwapInterval(_isVsync);
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return;
    }
    glViewport(_viewPort.x, _viewPort.y, _viewPort.z, _viewPort.w);

    Input::instance().init(p_glfwWindow);
    GUI::instance().init(this);
}

Application::~Application()
{
    glfwDestroyWindow(p_glfwWindow);
    glfwTerminate();
}

void Application::run()
{
    std::vector<Vertex> vertices = {
        // positions          // colors           // texture coords
        Vertex{
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        }, // top right
        Vertex{
            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
        }, // bottom right
        Vertex{
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
        }, // bottom left
        Vertex{
            glm::vec3(-0.5f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
        } // top left 
    };
    std::vector<GLuint> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<TextureMap> textures = {
        TextureMap(Texture::load("Resources/images/wall.jpg", false), TextureMapType::DIFFUSE),
        // TextureMap(Texture::load("Resources/images/awesomeface.png", false), TextureMapType::SPECULAR),
    };
    Mesh mesh(vertices, indices, textures);

    Shader shaderProgram1("Resources/GLSL/vertShader.glsl", "Resources/GLSL/fragShader.glsl");
    shaderProgram1.Activate();
    shaderProgram1.setInt("texture1", 1);
    shaderProgram1.setInt("texture2", 0);
    
    auto transform = glm::mat4(1.0);
    transform      = translate(transform, glm::vec3(0.0f, 0.0f, 10.0f));

    Camera             cam( this->_width, this->_height);
    CameraController cameraController(&cam);

	Shader guitarShader("Resources/GLSL/guitarShader.vert", "Resources/GLSL/guitarShader.frag");
    Model  model1("Resources/models/guitar/backpack.obj");
    TextureMap diffuse = TextureMap(Texture::load("Resources/models/guitar/diffuse.jpg"), TextureMapType::DIFFUSE);
    model1._material._diffuseMaps.push_back(diffuse);

    std::vector<std::string> skyBoxImg =
    {
        "Resources/images/Skybox/right.jpg",
        "Resources/images/Skybox/left.jpg",
        "Resources/images/Skybox/top.jpg",
        "Resources/images/Skybox/bottom.jpg",
        "Resources/images/Skybox/front.jpg",
        "Resources/images/Skybox/back.jpg"
    };
    Skybox skybox(skyBoxImg, "Resources/GLSL/Core/FrameBuffer/Skybox.vert",
        "Resources/GLSL/Core/FrameBuffer/Skybox.frag");

#pragma region my FBO
    Shader screenShader("Resources/GLSL/FrameBuffer.vert", "Resources/GLSL/FrameBuffer.frag");
    screenShader.Activate();
    screenShader.setInt("screenTexture", 0);

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    //     // positions        // texCoords
    //     -1.0f,  1.0f, 0.0,   0.0f, 1.0f,
    //     -1.0f, -1.0f, 0.0,   0.0f, 0.0f,
    //      1.0f, -1.0f, 0.0,   1.0f, 0.0f,
    //                   0.0,  
    //     -1.0f,  1.0f, 0.0,   0.0f, 1.0f,
    //      1.0f, -1.0f, 0.0,   1.0f, 0.0f,
    //      1.0f,  1.0f, 0.0,   1.0f, 1.0f
    // };
    // // screen quad VAO
    // unsigned int quadVAO, quadVBO;
    // glGenVertexArrays(1, &quadVAO);
    // glGenBuffers(1, &quadVBO);
    // glBindVertexArray(quadVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //


    // TODO: separate normal skybox and HDR sky box
#pragma region normal Skybox
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion 

#pragma region HDR Skybox
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader hdrShader("Resources/GLSL/Core/FrameBuffer/Hdr.vert",
        "Resources/GLSL/Core/FrameBuffer/Hdr.frag");
    hdrShader.setInt("hdrBuffer", 0);
#pragma endregion 
#pragma endregion 

    glfwSetTime(0.0);
    Time::instance()._lastUpdatedTime = 0.0;
    while(!glfwWindowShouldClose(p_glfwWindow))
    {
        // Calculate timer
        Time::instance()._time = glfwGetTime();
        Time::instance()._deltaTime = Time::instance()._time - Time::instance()._lastUpdatedTime;
        Time::instance()._lastUpdatedTime = Time::instance()._time;
              
        // Input
        Input::instance().update();
        cameraController.update();
        // start Render
        // Bind render data to FBO to quadVAO
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram1.Activate();
        shaderProgram1.setMat4("model", transform);
        shaderProgram1.setMat4("view", cam.getViewMatrix());
        shaderProgram1.setMat4("projection", cam.getPerspectiveProjectionMatrix());
        mesh.draw(shaderProgram1, cam, "", false);
        
        model1.draw(guitarShader, cam);

        // draw sky box here
        skybox.draw(cam);
        
        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        
        // glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // // clear all relevant buffers
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        // glClear(GL_DEPTH_BUFFER_BIT);
        //
        // // after bind FBO to quadVAO
        // // now render quadVAO
        // screenShader.Activate();
        // glBindVertexArray(quadVAO);
        // glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.Activate();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        hdrShader.setInt("hdr", hdr);
        hdrShader.setFloat("exposure", exposure);

        // after bind FBO to quadVAO
        // now render quadVAO
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
        
        // draw GUI
        GUI::instance().draw();
              
        glfwPollEvents();
        Input::instance().reset();
        glfwSwapBuffers(p_glfwWindow);
    }
}

void Application::setVsync(bool isOn)
{
    _isVsync = isOn;
    glfwSwapInterval(_isVsync);
}

GLuint Application::loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

