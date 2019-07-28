#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include <vector>
#include "List.h"
#include "loadshaders.h"

using namespace glm;
using namespace std;

class ParticleCluster {
public:
	ParticleCluster(int n, vec3 origin, vec3 color, float radius);
	~ParticleCluster();
	static void loadPrograms();
	static void setViewProjection(mat4 VP);
	static List<ParticleCluster*> particles;
	void setUniforms();
	void doStuff();
protected:
	GLuint VBO, TBO, VAOr, VAOt;
	static GLuint renderProgram, transformProgram;
	int particleCount;
	vec3 color, origin;
	float radius;
	int t;
};