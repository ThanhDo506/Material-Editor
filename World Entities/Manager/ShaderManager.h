#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "../../Render/Shader.h"
#include <unordered_map>

class ShaderManager
{
public:
    void addShader(Shader *shader, std::string const &shaderName);
    void removeShader(std::string const &shaderName);
    
    /**
	 * \brief 
	 * \param shaderName name of shader you want to get
	 * \param noLog if you don't want to throw an Exception when result is a nullptr
	 * \return pointer to Shader
	 */
	Shader* getShader(std::string const &shaderName, bool const &noLog = true);

    // TODO: This will need for future when create render pipeline
    void update();

	void clean();
private:
    std::unordered_map<std::string, Shader*> shaderMaps;

    friend class GUI;
};

#endif