#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include "../Entity.h"
#include "../../Render/Skybox.h"
#include "../../Core/Application.h"
#include "../../World Entities/Camera.h"
#include "../../World Entities/CameraController.h"
#include "../../Render/Model.h"

#include "LightManager.h"
#include "ShaderManager.h"

class WorldManager
{
public:
    static WorldManager& instance();

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
private:
    
#pragma region Singleton
    WorldManager(const WorldManager& obj) = delete;
    void operator=(const WorldManager& obj) = delete;
    WorldManager();
#pragma endregion

    void init(const Application * p_application);
    void update();
    void draw();
    void clean();
    
    std::list<Entity*>  _entities;
    
    LightManager        *p_lightManager;
    Skybox              *p_skybox;
    Camera              *p_mainCamera;
    CameraController    *p_mainCameraController;
    
    Application         *p_application;
    friend class Application;
    friend class GUI;
};

#endif