#include "Debris.h"

GLuint Debris::VAO;
List<Debris*> Debris::debris;

Debris::Debris(mat4 _Model, vec3 _vel, vec3 pos, float size, vec3 _color) : Polygon(pos, size) {
	Model = _Model;
	vel = _vel;
	color = _color;
}

void Debris::loadVertexArrays() {
	VAO = loadObjectNormalized("tetrahedron.txt");
}