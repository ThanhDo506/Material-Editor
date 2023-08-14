#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H
#include "Entity.h"
#include "../Render/Lighting/LightManager.h"

class WorldManager
{
public:
    static WorldManager& instance();

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
private:
    WorldManager(const WorldManager& obj) = delete;
    void operator=(const WorldManager& obj) = delete;
    WorldManager();
    ~WorldManager();

    void update();
    
    std::list<Entity*> _entities;
    LightManager _lightManager;
    
    friend class Application;
    friend class GUI;
};

#endif