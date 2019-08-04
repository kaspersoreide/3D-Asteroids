#pragma once
#include "Polygon.h"
#include "List.h"

class Debris : public Polygon {
public:
	Debris(mat4 Model, vec3 vel, vec3 pos, float size, vec3 color);
	void render() { Polygon::render(VAO); }
	static void loadVertexArrays();
	static List<Debris*> debris;
private:
	static GLuint VAO;
};