#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(glm::vec3 direction = glm::vec3(0.0,-1.0,0.0),
        glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f));

    LightType getLightType() const override;

    void setDirection(glm::vec3 direction);
    void setDiffuse(glm::vec3 diffuse);
    void setSpecular(glm::vec3 specular);
    void setAmbient(glm::vec3 ambient);

    glm::vec3 getDirection() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
    glm::vec3 getAmbient() const;

protected:
    std::string getUniformName(unsigned int lightIndex) const override;
    void updateShader(Shader &shader) override;

private:
    glm::vec3 _direction;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

    friend class GUI;
};

#endif