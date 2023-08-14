#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "Light.h"

class SpotLight : public Light
{
public:
    SpotLight(glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f),
              glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
              float innerAngle = glm::radians(10.0f),
              float outerAngle = glm::radians(19.5f),
              glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
              glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
              Attenuation attenuation = {1.0f, 0.09f, 0.032f});

    LightType getLightType() const override;

    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 position);
    glm::vec3 getDirection() const;
    void setDirection(glm::vec3 direction);
    glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 diffuse);
    glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 specular);
    Attenuation getAttenuation() const;
    void setAttenuation(Attenuation attenuation);

protected:
    std::string getUniformName(unsigned int lightIdx) const override;
    void updateShader(Shader& shader) override;

private:
    glm::vec3 _position;
    glm::vec3 _viewPosition;
    glm::vec3 _direction;
    glm::vec3 _viewDirection;

    float _innerAngle;
    float _outerAngle;

    glm::vec3 _diffuse;
    glm::vec3 _specular;

    Attenuation _attenuation;    
};

#endif