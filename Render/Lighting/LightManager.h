#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>

#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

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
};

#endif