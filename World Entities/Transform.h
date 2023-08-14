#ifndef TRANSFORM_H
#define TRANSFORM_H

#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Transform
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    static Transform identity();

    glm::mat4 getMatrixTransform() const;
    
    glm::vec3 forward() const;
    glm::vec3 up() const;
    glm::vec3 right() const;

    glm::vec3 eulerAngles() const;

    
    /**
     * \brief Quaternion from Euler Angles
     * \param pitch in degree
     * \param yaw in degree
     * \param roll in degree
     * \return Quaternion
     */
    static glm::quat eulerAnglesToQuaternion(float pitch, float yaw, float roll);
    
    /**
     * \brief Quaternion from Euler Angles
     * \param eulerAngles pitch, yaw, roll
     * \return Quaternion
     */
    static glm::quat eulerAnglesToQuaternion(glm::vec3 eulerAngles); 
};

#endif
