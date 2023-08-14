#include "WorldManager.h"

WorldManager& worldManager = WorldManager::instance();

WorldManager::WorldManager()
{
}

WorldManager::~WorldManager()
{
    _entities.erase(_entities.begin(), _entities.end());
}

void WorldManager::update()
{
    for (auto object : this->_entities)
    {
        if (object->_isActive)
        {
            object->update();
        }
    }
}

WorldManager& WorldManager::instance()
{
    static WorldManager instance;
    return instance;
}

void WorldManager::addEntity(Entity* entity)
{
    this->_entities.push_back(entity);
}

void WorldManager::removeEntity(Entity* entity)
{
    this->_entities.remove(entity);
}
