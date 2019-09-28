#include "Shooter.h"

GLuint Shooter::program;

Shooter::Shooter() {
	pos = dir = vel = vec3(0.0f);
	counter = 0;
	cooldown = 0;
	for (int i = 0; i < 2 * n; i++) {
		pos_vel[i] = vec3(0.0f);
	}
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pos_vel), pos_vel, GL_DYNAMIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, n * nicolasCage, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		6 * sizeof(float),			//stride (in bytes)
		(void*)0					//offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		6 * sizeof(float),			//stride (in bytes)
		(void*)(3 * sizeof(float))	//offset
	);
}

void Shooter::render() {
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos_vel), pos_vel);
	glBindVertexArray(VAO);
	glLineWidth(3.0f);
	glDrawArrays(GL_POINTS, 0, n);
}

void Shooter::shoot() {
	if (cooldown < 6) return;
	pos_vel[2 * counter] = pos + 0.5f * RNG::randomvec3() - vec3(0.25f);
	pos_vel[2 * counter + 1] = vel + 2.2f * dir + 0.1f * RNG::randomvec3() - vec3(0.05f);
	counter++;
	counter = counter % n;
	cooldown = 0;
}

void Shooter::collide(Polygon& p) {
	for (int i = 0; i < n; i++) {
		vec3 d = pos_vel[2 * i] - p.getPos();
		float l = length(d);
		if (l < p.getSize() && dot(pos_vel[2 * i + 1], d) < 0.0) {
			p.decrementHP(10);
			pos_vel[2 * i + 1] = 2.2f * d / l;
		}
	}
}

void Shooter::moveShots() {
	cooldown++;
	for (int i = 0; i < n; i++) {
		pos_vel[2 * i] = pos_vel[2 * i] + pos_vel[2 * i + 1];
	}
}

void Shooter::loadProgram() { 
	program = loadGeometryShader("shotVert.shader", "shotGeom.shader", "shotFrag.shader");
}

void Shooter::setPosAndDir(vec3 _pos, vec3 _dir) {
	pos = _pos;
	dir = _dir;
}

Shooter::~Shooter() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Shooter::setVP(mat4 VP) {
	glUseProgram(program);
	GLuint l_M = glGetUniformLocation(program, "VP");
	glUniformMatrix4fv(l_M, 1, GL_FALSE, &VP[0][0]);
}