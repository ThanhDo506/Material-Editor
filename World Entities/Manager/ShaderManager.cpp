#include "ShaderManager.h"
#include "../../utilities/utilities.h"

void ShaderManager::addShader(Shader* shader, std::string const &shaderName)
{
    try
    {
		if (!getShader(shaderName)) {
			throw Debug::Exception(Debug::WARNING, std::string("Had added " + shaderName).c_str());
		}
		shaderMaps.insert(std::pair<std::string, Shader*>(shaderName, shader));
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
			throw Debug::Exception(Debug::WARNING, shaderName + " doesn't exist");
		}
		shaderMaps.erase(it);
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
}

Shader* ShaderManager::getShader(std::string const& shaderName)
{
	std::unordered_map<std::string, Shader*>::const_iterator it = shaderMaps.find(shaderName);
	if (it == shaderMaps.end()) {
		AppLog("Counld not find" + shaderName);
		return nullptr;
	}
	return it->second;
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
