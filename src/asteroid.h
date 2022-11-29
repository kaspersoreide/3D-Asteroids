#pragma once
#include "polygon.h"
#include "random.h"
#include "particles.h"
#include <vector>
#include "convexhull.h"

class Asteroid : public Polygon {
public:
	Asteroid(vec3 pos, float size);
	Asteroid(Asteroid* parent, vector<vec3>& pts);
	~Asteroid() {}
	static void loadVertexArrays();
	void cleanup(vec3 playerPos);
	static void spawn(vec3 playerPos);
	void render();
	static void loadGlowShader();
	static void renderGlow(mat4 VP);
	void move();
	void explode();
	void split();
	bool detectCollision(Asteroid& a);
	void collide(Polygon& p) { Polygon::collide(p); }
	void collide(Asteroid& p);
	GLuint VAO;
	int numVertices;
	bool isAlive() { return alive; }
	static vector<Asteroid*> asteroids;
private:
	bool alive;
	float timeDead;
	vector<vec3> pointCloud;
	vector<Triangle> hull;
	static GLuint glowProgram, glowVAO;
};