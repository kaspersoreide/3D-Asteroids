#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "loadOBJ.h"
#include "loadshaders.h"
#include "matrix.h"

using namespace glm;

class Polygon {
public:
	static void loadProgram();
	Polygon() {}
	Polygon(vec3 _pos, float _size);
	void render(GLuint VAO);
	void move();
	void setSpin(vec3 _spin);
	void setVel(vec3 _vel) { vel = _vel; }
	bool collide(Polygon& p);
	void gravitate(Polygon& p);
	vec3 getPos() { return pos; }
	mat4 getModel() { return Model; }
	mat3 getRotation() { return Rotation; }
	void setRotation(mat3 R) { Rotation = R; }
	static void setViewProjection(mat4 VP);
	static void setCameraPosition(vec3 p);
protected:
	//all polygons share VAO, but not VBO
	//REMEMBER: buffer has to be loaded before attrib pointer
	//idk why
	static GLuint program;
	vec3 pos = vec3(0.0);
	vec3 spin = vec3(0.0);
	vec3 vel = vec3(0.0);
	mat3 Spin = mat3(1.0);
	mat3 Rotation = mat3(1.0);
	vec3 rot = vec3(6.28, 0.0, 0.0);
	mat4 Model = mat4(1.0f);
	mat4 Scale = mat4(1.0f);
	vec3 color;
	float size;
};