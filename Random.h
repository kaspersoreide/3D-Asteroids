#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\vec_swizzle.hpp>
#include <algorithm>

using namespace glm;

class RNG {
public:
	RNG() {}
	static unsigned int hash(unsigned int x);
	static unsigned int hash(unsigned int* x);
	static unsigned int randomUint();
	static float randomFloat();
	static unsigned int hash(uvec2 v);
	static unsigned int hash(uvec3 v);
	static unsigned int hash(uvec4 v);
	static float floatConstruct(unsigned int m);
	static float random(float x);
	static vec3 randomvec3();
	static float noise(vec2 p);
	static void srand(unsigned int s) { seed = s; }
private:
	static unsigned int seed;
};




