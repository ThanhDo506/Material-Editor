#include "LightManager.h"

#include <iostream>

void LightManager::addLight(Light* light)
{
    if(light == nullptr)
    {
        // TODO: throw Exception
        std::cerr << "LIGHT_MANAGER::ERROR::ADD_LIGHT::NULLPTR LIGHT\n";
        return;
    }
    if(!light->_isApply)
    {
        light->_isApply = true;
        _lights.push_back(light);
        switch (light->getLightType())
        {
        case Light::LightType::DIRECTIONAL_LIGHT:
            light->setLightIndex(_directionalCount);
            _directionalCount++;
            break;
        case Light::LightType::POINT_LIGHT:
            light->setLightIndex(_pointCount);
            _pointCount++;
            break;
        case Light::LightType::SPOT_LIGHT:
            light->setLightIndex(_spotCount);
            _spotCount++;
            break;
        default:
            // TODO: throw Exception here
            std::cout << "LIGHT_MANAGER::ERROR::ADD_LIGHT::UNKNOWN TYPE OF LIGHT\n";
            break;
        }
    } else
    {
        // TODO: throw Exception here
        std::cout << "LIGHT_MANAGER::ERROR::ADD_LIGHT::LIGHT HAD BEEN APPLIED\n";
    }
}

void LightManager::removeLight(Light* light)
{
    switch (light->getLightType())
    {
    case Light::LightType::DIRECTIONAL_LIGHT:
        light->setLightIndex(_directionalCount);
        _directionalCount--;
        break;
    case Light::LightType::POINT_LIGHT:
        light->setLightIndex(_pointCount);
        _pointCount--;
        break;
    case Light::LightType::SPOT_LIGHT:
        light->setLightIndex(_spotCount);
        _spotCount--;
    default:
        std::cout << "LIGHT_MANAGER::ADD_ERROR::UNKNOWN TYPE OF LIGHT\n";
        break;
    }
    auto it = std::find(_lights.begin(), _lights.end(), light);
    if(it != _lights.end())
    {
        _lights.erase(it);
        delete light;
    } else
    {
        throw new std::string("Light isn't signed up in LightManager\n");
    }
}

void LightManager::updateShader(Shader& shader)
{
    shader.Activate();
    shader.setInt("DirectionalLightCount", static_cast<int>(_directionalCount));
    shader.setInt("PointLightCount", static_cast<int>(_pointCount));
    shader.setInt("SpotLightCount", static_cast<int>(_spotCount));
    for (auto light : _lights)
    {
        light->updateShader(shader);
    }
}

void LightManager::clean()
{
    for (auto light : _lights)
    {
        delete light;
    }
}
