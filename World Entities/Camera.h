#pragma once
#include "Entity.h"
#include "Transform.h"
#include "../Render/Shader.h"

class Camera : public Entity
{
public:
    /**
     * \brief 
     * \param transform 
     * \param screenWidth 
     * \param screenHeight 
     * \param near 
     * \param far 
     * \param zoom fov
     */
    Camera(int       screenWidth,
               int       screenHeight,
               Transform transform = Transform::identity(),
               float     near        = 0.1f,
               float     far         = 1000.f,
               float     zoom        = 45.0f);
    glm::mat4 getViewMatrix() const;

    glm::mat4 getPerspectiveProjectionMatrix() const;
    glm::mat4 getOrthoProjectionMatrix() const;
#pragma region Getter setter

    float getZoom() const
    {
        return _zoom;
    }

    void setZoom(float& zoom)
    {
        _zoom = zoom;
    }

    float getNear() const
    {
        return _near;
    }

    void setNear(float& near)
    {
        _near = near;
    }

    float getFar() const
    {
        return _far;
    }

    void setFar(float& far)
    {
        _far = far;
    }

    int getWidth() const
    {
        return _width;
    }

    void setwidth(int& width)
    {
        _width = width;
    }
#pragma endregion
private:
    float _zoom;
    float _near;
    float _far;

    int _width, _height;

    void init() override;
    void update() override;

    friend class Application;
    friend class CameraController;
    friend class GUI;
};
