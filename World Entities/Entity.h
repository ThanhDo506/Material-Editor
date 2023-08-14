#pragma once
#include <list>
#include <string>
#include <glm/glm.hpp>
#include "Transform.h"

class Entity
{
public:
    Entity(Transform transform = Transform::identity(), std::string name = "Entity", Entity* parent = nullptr);
    virtual ~Entity();
    Entity* getParent();
    Entity* findInChild(const std::string& name);
    void    setParent(Entity* parent);

    glm::mat4 getWorldMatrixTransform() const;

    Transform          transform;
    std::string        _name;
    bool               _isActive = true;
    Entity*            p_parent;
    std::list<Entity*> _children;

    virtual void init() = 0;
    virtual void update() = 0;

    friend class WorldManager;
};
