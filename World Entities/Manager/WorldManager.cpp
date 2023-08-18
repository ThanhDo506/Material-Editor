#include "WorldManager.h"
#include <iostream>
#include "../../utilities/utilities.h"

WorldManager& worldManager = WorldManager::instance();

WorldManager::WorldManager()
{
    p_lightManager = nullptr;
    p_skybox = nullptr;
    p_mainCamera = nullptr;
}

void WorldManager::init(Application * p_application)
{
    p_lightManager = new LightManager;
    const std::vector<std::string> skyBoxImg =
    {
        "Resources/images/Skybox/right.jpg",
        "Resources/images/Skybox/left.jpg",
        "Resources/images/Skybox/top.jpg",
        "Resources/images/Skybox/bottom.jpg",
        "Resources/images/Skybox/front.jpg",
        "Resources/images/Skybox/back.jpg"
    };
    p_skybox = new Skybox(skyBoxImg, "Resources/GLSL/Core/FrameBuffer/Skybox.vert",
        "Resources/GLSL/Core/FrameBuffer/Skybox.frag");

    p_mainCamera = new Camera(p_application->getWidth(), p_application->getHeight());
    p_mainCameraController = new CameraController(p_mainCamera);
    p_shaderManager = new ShaderManager;
}

void WorldManager::update()
{
    p_mainCameraController->update();
    for (auto object : _entities)
    {
        if (object->_isActive)
        {
            object->update();
        }
    }
}

void WorldManager::draw()
{
    try
    {
        if(!p_mainCamera)
        {
			throw Debug::Exception(Debug::ERROR, "Main camera is null");
        }
        
        // draw skybox
        glDisable(GL_CULL_FACE);
        p_skybox->draw(*p_mainCamera);

        // draw Entity
        for(auto entity : _entities)
        {
            auto model = dynamic_cast<Model*>(entity);
            if(entity->_isActive && model)
            {
                model->draw(*p_mainCamera);
            }
        }
    } catch (std::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void WorldManager::clean()
{
    _entities.clear();
    
    p_lightManager->clean();
    delete p_lightManager;
    p_lightManager = nullptr;

    p_skybox->clean();
    delete p_skybox;
    p_skybox = nullptr;

    delete p_mainCamera;
    p_mainCamera = nullptr;

    delete p_mainCameraController;
    p_mainCameraController = nullptr;

    p_shaderManager->clean();
    delete p_shaderManager;
    p_shaderManager = nullptr;

    p_application = nullptr;
}

WorldManager& WorldManager::instance()
{
    static WorldManager instance;
    return instance;
}

void WorldManager::addEntity(Entity* entity)
{
    _entities.push_back(entity);
}

void WorldManager::removeEntity(Entity* entity)
{
    _entities.remove(entity);
}
