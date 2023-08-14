#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float innerAngle, float outerAngle, glm::vec3 diffuse,
    glm::vec3 specular, Attenuation attenuation)
{
    position = _position;
    direction = _direction;
    innerAngle = _innerAngle;
    outerAngle = _outerAngle;
    specular = _specular;
    attenuation = _attenuation;
}

Light::LightType SpotLight::getLightType() const
{
    return LightType::SPOT_LIGHT;
}

glm::vec3 SpotLight::getPosition() const
{
    return _position;
}

void SpotLight::setPosition(glm::vec3 position)
{
    _position = position;
}

glm::vec3 SpotLight::getDirection() const
{
    return _direction;
}

void SpotLight::setDirection(glm::vec3 direction)
{
    _direction = direction;
}

glm::vec3 SpotLight::getDiffuse() const
{
    return _diffuse;
}

void SpotLight::setDiffuse(glm::vec3 diffuse)
{
    _diffuse = diffuse;
}

glm::vec3 SpotLight::getSpecular() const
{
    return _specular;
}

void SpotLight::setSpecular(glm::vec3 specular)
{
    _specular = specular;
}

Light::Attenuation SpotLight::getAttenuation() const
{
    return _attenuation;
}

void SpotLight::setAttenuation(Attenuation attenuation)
{
    _attenuation = attenuation;
}

std::string SpotLight::getUniformName(unsigned lightIdx) const
{
    return "_SpotLights[" + std::to_string(lightIdx) + "]";

}

void SpotLight::updateShader(Shader& shader)
{
    shader.setVec3(_uniformName + ".position", _position);
    shader.setFloat(_uniformName + ".innerAngle", _innerAngle);
    shader.setFloat(_uniformName + ".outerAngle", _outerAngle);
    shader.setVec3(_uniformName + ".diffuse", _diffuse);
    shader.setVec3(_uniformName + ".specular", _specular);
    shader.setFloat(_uniformName + ".attenuation.constant", _attenuation.constant);
    shader.setFloat(_uniformName + ".attenuation.linear", _attenuation.linear);
    shader.setFloat(_uniformName + ".attenuation.quadratic", _attenuation.quadratic);
}
