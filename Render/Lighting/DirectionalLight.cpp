#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 diffuse, glm::vec3 specular)
{
    _direction = direction;
    _diffuse = diffuse;
    _specular = specular;
}

Light::LightType DirectionalLight::getLightType() const
{
    return LightType::DIRECTIONAL_LIGHT;
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
    _direction = direction;
}

void DirectionalLight::setDiffuse(glm::vec3 diffuse)
{
    _diffuse = diffuse;
}

void DirectionalLight::setSpecular(glm::vec3 specular)
{
    _specular = specular;
}

glm::vec3 DirectionalLight::getDirection() const
{
    return _direction;
}

glm::vec3 DirectionalLight::getDiffuse() const
{
    return _diffuse;
}

glm::vec3 DirectionalLight::getSpecular() const
{
    return _specular;
}

std::string DirectionalLight::getUniformName(unsigned lightIndex) const
{
    return "_DirectionalLights[" + std::to_string(_index) + "]";
}

void DirectionalLight::updateShader(Shader& shader)
{
    shader.setVec3(_uniformName + ".direction", _direction);
    shader.setVec3(_uniformName + ".specular", _specular);
    shader.setVec3(_uniformName + ".diffuse", _diffuse);
}