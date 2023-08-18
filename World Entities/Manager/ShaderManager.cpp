#include "ShaderManager.h"
#include "../../utilities/utilities.h"

void ShaderManager::addShader(Shader* shader, std::string const &shaderName)
{
    try
    {
		if (getShader(shaderName) != nullptr) {
			throw Debug::Exception(Debug::WARNING, "SHADER_MANAGER::Shader had been added");
		}
		shaderMaps.insert(std::pair<std::string, Shader*>(shaderName, shader));
    	AppLog("SHADER_MANAGER::Added \"" + shaderName + "\" shader");
    }
    catch (std::exception const &e)
    {
		std::cerr << e.what() << std::endl;
    }
}

void ShaderManager::removeShader(std::string const& shaderName)
{
	std::unordered_map<std::string, Shader*>::const_iterator it = shaderMaps.find(shaderName);
	try {
		if (it == shaderMaps.end()) {
			throw Debug::Exception(Debug::WARNING, "SHADER_MANAGER::" + shaderName + " doesn't exist");
		}
		shaderMaps.erase(it);
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
}

Shader* ShaderManager::getShader(std::string const& shaderName, bool const &noLog)
{
	try
	{
		std::unordered_map<std::string, Shader*>::const_iterator it = shaderMaps.find(shaderName);
		if (it == shaderMaps.end()) {
			if(noLog)
				return nullptr;
			throw Debug::Exception(Debug::WARNING, "SHADER_MANAGER::Could not find shader \"" + shaderName + "\"");
		}
		return it->second;
	} catch (std::exception const &e)
	{
		std::cout << e.what() << std::endl;
	}
	return nullptr;
}

void ShaderManager::update()
{
}

void ShaderManager::clean()
{
	for (auto& x : shaderMaps) {
		x.second->Delete();
		delete x.second;
	}
	shaderMaps.clear();
}
