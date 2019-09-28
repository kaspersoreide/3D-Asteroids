#pragma once
#include "Polygon.h"
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
	static void loadGlowShader();
	static void renderGlow(mat4 VP);
	void move();
	void explode();
	void collide(Polygon& p) { Polygon::collide(p); }
	void collide(Asteroid& p);
	bool isAlive() { return alive; }
	static List<Asteroid*> asteroids;
private:
	bool alive;
	float timeDead;
	static GLuint VAO, glowProgram, glowVAO;
};