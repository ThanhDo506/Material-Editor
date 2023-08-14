#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include "Camera.h"
#include <GLFW/glfw3.h>
#include "../Core/Input.h"
#include "../Core/Time.h"
class CameraController
{
public:
    CameraController(Camera *camera, float speed = 3.5f,
        float sensitivityX = 200.0f,
        float sensitivityY = 200.0f);
    
private:
    bool    _isForcus = false;
    float   _speed;

    float _sensitivityX;
    float _sensitivityY;
    float rotationX;
    float rotationY;

    void cameraMovement();
    void cameraRotation();
    void update();
    
    Camera *p_camera;

    friend class Application;
    friend class GUI;
};

#endif