#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "loadshaders.h"
#include "Random.h"
#include <vector>

using namespace glm;

class Skybox
{
public:
	static void Init();
	static void render();
	static void setViewProjection(mat3 Rotation, mat4 Projjection);
	Skybox();
	~Skybox();
private:
	static GLuint VBO, VAO, program;
	static int N;
};

