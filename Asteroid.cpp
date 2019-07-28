#include "Asteroid.h"

GLuint Asteroid::VAOlist[4];
List<Asteroid*> Asteroid::asteroids;

Asteroid::Asteroid(vec3 pos, float size) : Polygon(pos, size) { 
	color = RNG::randomvec3(); 
	setSpin(RNG::randomvec3() * 0.06f - vec3(0.03f));
	VAO = VAOlist[RNG::randomUint() % 4];
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
		a->setVel(-0.3f * (normalize(a->pos - playerPos) + RNG::randomvec3() * 0.8f - vec3(0.4f)));
		asteroids.push_back(a);
	}
}

void Asteroid::loadVertexArrays() {
	VAOlist[0] = loadObjectNoNormals("gyroelongated.txt");
	VAOlist[1] = loadObjectNoNormals("triaugtriprism.txt");
	VAOlist[2] = loadObjectNoNormals("icosahedron.txt");
	VAOlist[3] = loadObjectNoNormals("greatdodeca.txt");
}

void Asteroid::render() {
	Polygon::render(VAO);
}

void Asteroid::move() {
	//vel -= 0.001f * pos / dot(pos, pos);
	Polygon::move();
}

void Asteroid::explode() {
	vec3 p1 = normalize(RNG::randomvec3()) * (0.8f * size);

	Asteroid* a1 = new Asteroid(pos + p1, size / 2);
	a1->setVel(vel + cross(spin, p1) + 0.2f * normalize(p1));
	a1->color = color;

	Asteroid* a2 = new Asteroid(pos - p1, size / 2);
	a2->setVel(vel + cross(spin, -p1) - 0.2f * normalize(p1));
	a2->color = color;

	ParticleCluster::particles.push_back(
		new ParticleCluster((int)(size * 50), pos, color, size)
	);
	asteroids -= this;
	asteroids.push_back(a1);
	asteroids.push_back(a2);
}

void Asteroid::collide(Asteroid& p) {
	if (Polygon::collide(p)) {
		if (size > p.size) {
			explode();
			asteroids -= &p;
		}
		else {
			p.explode();
			asteroids -= this;
		}
	}
}