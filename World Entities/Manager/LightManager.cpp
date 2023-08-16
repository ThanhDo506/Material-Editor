#include "LightManager.h"

#include <iostream>

#include "../../utilities/utilities.h"

void LightManager::addLight(Light* light)
{
	try {
		if (light == nullptr)
		{
			throw Debug::Exception(Debug::ERROR, "NULLPTR LIGHT");
		}
		if (!light->_isApply)
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
				throw Debug::Exception(Debug::WARNING, "UNKNOWN TYPE OF LIGHT");
				break;
			}
		}
		else
		{
			throw Debug::Exception(Debug::WARNING, "LIGHT HAD BEEN APPLIED");
		}
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
}

void LightManager::removeLight(Light* light)
{
    try
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
            throw Debug::Exception(Debug::DISPLAY_TYPE::WARNING, "UNKNOWN TYPE OF LIGHT");
        }
        auto it = std::find(_lights.begin(), _lights.end(), light);
        if(it != _lights.end())
        {
            _lights.erase(it);
            delete light;
        } else
        {
            throw Debug::Exception(Debug::DISPLAY_TYPE::WARNING, "Light isn't signed up in LightManager");
        }
    } catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
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
