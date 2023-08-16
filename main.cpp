#include <iostream>
#include <glm/glm.hpp>
#include "Core/Application.h"
#include "utilities/utilities.h"

int main()
{
	Application* app = new Application("Learn OpenGL", 1920, 1080, glm::vec4(0,0,1920,1080));
	app->run();
}
