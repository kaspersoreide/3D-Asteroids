#pragma once
#include "Polygon.h"
#include "Asteroid.h"

class Player : public Polygon {
public:
	static void loadVertexArray();
	Player();
	void render();
	void move();
	Asteroid* shoot();
	mat4 getView();
	bool rot[6], mov[6];
private:
	static GLuint VAO;
};