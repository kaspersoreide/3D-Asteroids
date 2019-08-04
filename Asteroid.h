#pragma once
#include "Polygon.h"
#include "Debris.h"
#include "Random.h"
#include "Particles.h"
#include "List.h"

class Asteroid : public Polygon {
public:
	Asteroid(vec3 pos, float size);
	~Asteroid() {}
	static void loadVertexArrays();
	void cleanup(vec3 playerPos);
	static void spawn(vec3 playerPos);
	void render();
	void move();
	void explode();
	void collide(Polygon& p) { Polygon::collide(p); }
	void collide(Asteroid& p);
	
	static List<Asteroid*> asteroids;
private:
	static GLuint VAOlist[1];
	GLuint VAO;
};