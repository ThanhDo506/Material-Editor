#include "Transform.h"

glm::mat4 Transform::getMatrixTransform() const
{
    glm::mat4 positionMtx = glm::translate(glm::mat4(1.0), position);
    glm::mat4 scaleMtx = glm::scale(glm::mat4(1.0), scale);
    glm::mat4 rotationMtx = glm::toMat4(rotation);
    return positionMtx * rotationMtx * scaleMtx;
}

Transform Transform::identity()
{
    return Transform {
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(1.0, 1.0, 1.0),
        glm::quat(1.0, 0.0, 0.0, 0.0)
    };
}

glm::vec3 Transform::forward() const
{
    return rotation * glm::vec3(0.0, 0.0, 1.0);
}

glm::vec3 Transform::up() const
{
    return rotation * glm::vec3(0.0, 1.0, 0.0);
}

glm::vec3 Transform::right() const
{
    return rotation * glm::vec3(1.0, 0.0, 0.0);
}

glm::vec3 Transform::eulerAngles() const
{
    return glm::degrees(glm::eulerAngles(rotation));
}

glm::quat Transform::eulerAnglesToQuaternion(float pitch, float yaw, float roll)
{
    return glm::quat(glm::vec3(
        glm::radians(pitch),
        glm::radians(yaw),
        glm::radians(roll)));
}

glm::quat Transform::eulerAnglesToQuaternion(glm::vec3 eulerAngles)
{
    return eulerAnglesToQuaternion(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}
