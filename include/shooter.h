#pragma once
#include "asteroid.h"

class Shooter {
private:
	static constexpr int n = 20;
	static GLuint program;
	int counter;
	vec3 pos_vel[2 * n];
	vec3 pos, dir, vel;
	int cooldown;
	GLuint VBO, VAO;
public:
	Shooter();
	~Shooter();
	void shoot();
	void moveShots();
	void collide(Polygon& p);
	void render();
	void setPosAndDir(vec3 _pos, vec3 _dir);
	void setVel(vec3 _vel) { vel = _vel; }
	static void loadProgram();
	static void setVP(mat4 VP);
};