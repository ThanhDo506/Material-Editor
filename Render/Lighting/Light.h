#ifndef LIGHT_H
#define LIGHT_H
#include <string>
#include "../Shader.h"

class Light
{
public:
    enum class LightType
    {
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT
    };
    
    struct Attenuation
    {
        float constant;
        float linear;
        float quadratic;
    };
    
    virtual ~Light() = default;
    virtual LightType getLightType() const = 0;
    virtual void updateShader(Shader& shader) = 0;

    virtual std::string getUniformName(unsigned int lightIdx) const = 0;
    inline void setLightIndex(unsigned int index)
    {
        _index = index;
        _uniformName = getUniformName(_index);
    }
    
    // is this light applied to LightManager
    inline bool isApply() const { return _isApply; }
protected:
    // is this light applied to LightManager
    bool         _isApply = false;
    unsigned int _index;
    // this will be set in LightManager
    std::string _uniformName;

    friend class LightManager;
    friend class Application;
};

#endif
