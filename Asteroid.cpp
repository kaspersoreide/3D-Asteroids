#include "Asteroid.h"

GLuint Asteroid::VAO;
List<Asteroid*> Asteroid::asteroids;

Asteroid::Asteroid(vec3 pos, float size) : Polygon(pos, size) { 
	color = RNG::randomvec3(); 
	alive = true;
	timeDead = 0.0f;
};

void Asteroid::cleanup(vec3 playerPos) {
	vec3 d = pos - playerPos;
	if (dot(d, d) > 100000.0f) {
		asteroids -= this;
	}
}

void Asteroid::spawn(vec3 playerPos) {
	if (asteroids.size() < 18) {
		Asteroid* a = new Asteroid(
			playerPos + 100.0f * normalize(2.0f * RNG::randomvec3() - 1.0f),
			0.5f + 5.0f * RNG::randomFloat()
		);
		a->setSpin(RNG::randomvec3() * 0.06f - vec3(0.03f));
		a->setVel(-0.3f * (normalize(a->pos - playerPos) + RNG::randomvec3() * 0.8f - vec3(0.4f)));
		asteroids.push_back(a);
	}
}

void Asteroid::loadVertexArrays() {
	VAO = loadObjectNormalized("asteroid1.txt");
}

void Asteroid::render() {
	//Polygon::render(VAO);
	glBindVertexArray(VAO);
	glUseProgram(program);
	GLuint l_M = glGetUniformLocation(program, "Model");
	glUniformMatrix4fv(l_M, 1, GL_FALSE, &Model[0][0]);
	GLuint l_c = glGetUniformLocation(program, "pColor");
	glUniform3fv(l_c, 1, &color[0]);
	GLuint l_a = glGetUniformLocation(program, "alive");
	glUniform1i(l_a, alive);
	if (!alive) {
		GLuint l_t = glGetUniformLocation(program, "t");
		glUniform1f(l_t, timeDead);
		GLuint l_s = glGetUniformLocation(program, "spin");
		glUniform3fv(l_s, 1, &spin[0]);
	}
	//glDrawArrays(GL_TRIANGLES, 0, 60);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 80);
}

void Asteroid::move() {
	//vel -= 0.001f * pos / dot(pos, pos);
	if (HP < 0 && alive) explode();
	Polygon::move();
	if (!alive) {
		timeDead += 0.1;
		if (timeDead > 20.0) asteroids -= this;
	}
}

void Asteroid::explode() {
	alive = false;
	//vel = vec3(0.0f);
	Spin = mat3(1.0);
	ParticleCluster::particles.push_back(
		new ParticleCluster(400, pos, color, size)
	);
}

void Asteroid::collide(Asteroid& p) {
	if (detectCollision(p)) {
		/*if (size > p.size) {
			explode();
			asteroids -= &p;
		}
		else {
			p.explode();
			asteroids -= this;
		}*/
		if (length(vel - p.vel) > 1.0f) {
			if (size > p.size) {
				explode();
			}
			else {
				p.explode();
			}
		}
		else {
			Polygon::collide(p);
		}
	}
}