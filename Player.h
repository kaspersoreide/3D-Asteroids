#pragma once
#include "Polygon.h"
#include "Asteroid.h"
#include "Shooter.h"

class Player : public Polygon {
public:
	static void loadVertexArray();
	Player();
	~Player() { delete shooter; }
	void render();
	void move();
	void shoot();
	void collideShots(Polygon& p) { shooter->collide(p); }
	mat4 getView();
	bool rot[6], mov[6], shooting;
private:
	static GLuint VAO;
	Shooter* shooter;
};