#include "Asteroid.h"

GLuint Asteroid::VAOlist[1];
List<Asteroid*> Asteroid::asteroids;

Asteroid::Asteroid(vec3 pos, float size) : Polygon(pos, size) { 
	color = RNG::randomvec3(); 
	setSpin(RNG::randomvec3() * 0.06f - vec3(0.03f));
	VAO = VAOlist[0];
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
	VAOlist[0] = loadObjectNormalized("icosahedron.txt");
}

void Asteroid::render() {
	Polygon::render(VAO);
}

void Asteroid::move() {
	//vel -= 0.001f * pos / dot(pos, pos);
	Polygon::move();
}

void Asteroid::explode() {
	//vec3 p1 = normalize(RNG::randomvec3()) * (0.8f * size);
	Debris* d[20];
	const vec3 dirs[3] = {
		normalize(vec3(-0.5, 0.0, -0.353553) + vec3(0.5, 0.0, -0.353553)),
		normalize(vec3(0.0, 0.5, 0.353553) + vec3(0.5, 0.0, -0.353553)),
		normalize(vec3(0.0, -0.5, 0.353553) + vec3(0.5, 0.0, -0.353553))
	};
	mat3 initialRotation = srotate(0.5235987f * vec3(0.0, 1.0, 0.0));
	mat3 debrisRotation = initialRotation;
	const vec3 p = 0.5f * normalize(vec3(0.5, 0.0, -0.353553));
	mat4 debrisModel = translateR(
		mat3(0.5f) * initialRotation,
		p);

	for (int i = 0; i < 20; i++) {
		vec3 relPos = debrisModel * vec4(0.0, 0.0, 0.0, 1.0);
		d[i] = new Debris(Model * debrisModel, vec3(0.0f), pos + relPos, size / 2, color);
		d[i]->setRotation(debrisRotation);
		Debris::debris.push_back(d[i]);
		int way = (i % 2 == 0) ? 1 : -1;
		const float fifth = 1.256637f;
		debrisRotation *= srotate(way * fifth * debrisRotation * dirs[i % 3]);
		debrisModel = translateR(mat3(0.5) * debrisRotation, debrisRotation * p);
	}
	//Debris* d1 = new Debris(Model, vel, pos + p1, size / 2, color);
	//Debris* d2 = new Debris(Model, vel, pos - p1, size / 2, color);

	ParticleCluster::particles.push_back(
		new ParticleCluster((int)(size * 50), pos, color, size)
	);
	asteroids -= this;
	//Debris::debris.push_back(d1);
	//Debris::debris.push_back(d2);
}

void Asteroid::collide(Asteroid& p) {
	if (Polygon::collide(p)) {
		/*if (size > p.size) {
			explode();
			asteroids -= &p;
		}
		else {
			p.explode();
			asteroids -= this;
		}*/
		if (size > p.size) {
			explode();
		}
		else {
			p.explode();
		}
	}
}