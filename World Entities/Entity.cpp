#include "Entity.h"

Entity::Entity(Transform transform, std::string name, Entity* parent)
{
    _name      = name;
    p_parent   = parent;
    this->transform = transform;
    if (p_parent != nullptr)
        p_parent->_children.push_back(this);
}

Entity::~Entity()
{
    p_parent = nullptr;
    _children.clear();
}

Entity* Entity::getParent()
{
    return p_parent;
}

Entity* Entity::findInChild(const std::string& name)
{
    if (this->_name == name)
    {
        return this;
    }
    for (Entity* child : this->_children)
    {
        Entity* res = child->findInChild(name);
        if (res != nullptr)
        {
            return res;
        }
    }
    return nullptr;
}

void Entity::setParent(Entity* parent)
{
    if (p_parent != nullptr)
        p_parent->_children.remove(this);
    p_parent = parent;
    p_parent->_children.insert(p_parent->_children.begin(), this);
}

glm::mat4 Entity::getWorldMatrixTransform() const
{
    glm::mat4   worldMatrixTransform = transform.getMatrixTransform();
    Entity* acestor = p_parent;
    while (acestor == nullptr)
    {
        worldMatrixTransform = acestor->transform.getMatrixTransform() * worldMatrixTransform;
        acestor        = acestor->p_parent;
    }
    return worldMatrixTransform;
}
