#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Asteroid.h"

using namespace glm;

class HUD
{
public:
	HUD();
	~HUD();

private:
	GLuint program;
};

