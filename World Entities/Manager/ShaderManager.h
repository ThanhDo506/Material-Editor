#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "../../Render/Shader.h"
#include <unordered_map>

class ShaderManager
{
public:
    void addShader(Shader *shader, std::string const &shaderName);
    void removeShader(std::string const &shaderName);

	Shader* getShader(std::string const &shaderName);

    // TODO: This will need for future when create render pipeline
    void update();

	void clean();
private:
    std::unordered_map<std::string, Shader*> shaderMaps;

};

#endif