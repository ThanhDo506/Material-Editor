#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular, Attenuation attenuation)
{
    _attenuation = attenuation;
    _position = position;
    _diffuse = diffuse;
    _specular = specular;
}

Light::LightType PointLight::getLightType() const
{
    return LightType::POINT_LIGHT;
}

glm::vec3 PointLight::getPosition() const
{
    return _position;
}

void PointLight::setPosition(glm::vec3 position)
{
    _position = position;
}

glm::vec3 PointLight::getDiffuse() const
{
    return _diffuse;
}

void PointLight::setDiffuse(glm::vec3 diffuse)
{
    _diffuse = diffuse;
}

glm::vec3 PointLight::getSpecular() const
{
    return _specular;
}

void PointLight::setSpecular(glm::vec3 specular)
{
    _specular = specular;
}

Light::Attenuation PointLight::getAttenuation() const
{
    return _attenuation;
}

void PointLight::setAttenuation(Attenuation attenuation)
{
    _attenuation = attenuation;
}

std::string PointLight::getUniformName(unsigned int lightIdx) const
{
    return "_PointLights[" + std::to_string(lightIdx) + "]";
}

void PointLight::updateShader(Shader& shader)
{
    shader.setVec3(_uniformName + ".diffuse", _diffuse);
    shader.setVec3(_uniformName + ".specular", _specular);
    shader.setFloat(_uniformName + ".attenuation.constant", _attenuation.constant);
    shader.setFloat(_uniformName + ".attenuation.linear", _attenuation.linear);
    shader.setFloat(_uniformName + ".attenuation.quadratic", _attenuation.quadratic);
}
