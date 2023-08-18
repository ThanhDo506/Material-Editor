#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>

#include "../../Render/Lighting/Light.h"
#include "../../Render/Lighting/PointLight.h"
#include "../../Render/Lighting/SpotLight.h"
#include "../../Render/Lighting/DirectionalLight.h"

class LightManager
{
public:
    void addLight(Light *light);
    void removeLight(Light *light);
    void updateShader(Shader &shader);
private:
    unsigned int _directionalCount = 0;
    unsigned int _pointCount = 0;
    unsigned int _spotCount = 0;
    std::vector<Light*> _lights;

    void clean();
    friend class Application;
    friend class GUI;
    friend class WorldManager;
};

#endif