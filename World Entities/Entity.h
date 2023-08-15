#pragma once
#include <list>
#include <string>
#include <glm/glm.hpp>
#include "Transform.h"
#include "../Render/Shader.h"

class Entity
{
public:
    Entity(Transform transform = Transform::identity(), std::string name = "Entity", Entity* parent = nullptr);
    virtual ~Entity();
    Entity* getParent();
    Entity* findInChild(const std::string& name);
    void    setParent(Entity* parent);

    glm::mat4 getWorldMatrixTransform() const;

    bool                _isActive = true;
    Transform           transform;
    std::string         _name;
    Entity              *p_parent;
    Shader              *p_shader;
    std::list<Entity*>  _children;

    virtual void init() = 0;
    virtual void update() = 0;

    friend class WorldManager;
};
