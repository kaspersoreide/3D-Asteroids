#include "asteroid.h"
#include "vertexarrays.h"

GLuint Asteroid::glowProgram, Asteroid::glowVAO;
vector<Asteroid*> Asteroid::asteroids;

Asteroid::Asteroid(Asteroid* parent, vector<vec3>& pts) {
	pointCloud = pts;
	vec3 sum(.0f);
	for (vec3 p : pointCloud) {
		sum += p;
	}
	vec3 center = sum / float(pointCloud.size());
	for (auto it = pointCloud.begin(); it != pointCloud.end(); ++it) {
		*it -= center;
	}
	hull = giftWrap(pointCloud);
	vector<vec3> vertices;
	for (Triangle t : hull) {
		vertices.push_back(t.pts[0]);
		vertices.push_back(t.normal);
		vertices.push_back(t.pts[1]);
		vertices.push_back(t.normal);
		vertices.push_back(t.pts[2]);
		vertices.push_back(t.normal);
	}
	VAO = vertexArray2x3f(vertices);
	Rotation = parent->Rotation;
	pos = parent->pos + (Rotation * center);
	size = parent->size / 1.5f;
	Scale = parent->Scale;
	//setSpin(parent->spin);
	vec3 spinvel = cross(parent->spin, center);
	vel = spinvel + parent->vel + 0.1f * (Rotation * center);
	setSpin(parent->spin - spinvel);
	alive = true;
	timeDead = 0.0f;
	HP = int(20 * size);
	numVertices = vertices.size() / 2;
	color = parent->color;
}

Asteroid::Asteroid(vec3 pos, float size) : Polygon(pos, size) { 
	for (int i = 0; i < 300; i++) {
		vec3 p = 2.f * RNG::randomvec3() - vec3(1.f);
		p = normalize(p) * RNG::randomFloat();
		pointCloud.push_back(p);
	}
	hull = giftWrap(pointCloud);
	vector<vec3> vertices;
	for (Triangle t : hull) {
		vertices.push_back(t.pts[0]);
		vertices.push_back(t.normal);
		vertices.push_back(t.pts[1]);
		vertices.push_back(t.normal);
		vertices.push_back(t.pts[2]);
		vertices.push_back(t.normal);
	}
	//VAO = loadObjectNormalized("icosahedron.txt");
	numVertices = vertices.size() / 2;
	VAO = vertexArray2x3f(vertices);
	color = RNG::randomvec3(); 
	alive = true;
	timeDead = 0.0f;
};

void Asteroid::cleanup(vec3 playerPos) {
	vec3 d = pos - playerPos;
	if (dot(d, d) > 100000.0f) {
		asteroids.erase(find(asteroids.begin(), asteroids.end(), this));
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
	//VAO = loadObjectNormalized("icosahedron.txt");
}

void Asteroid::render() {
	//Polygon::render(VAO);
	if (!alive) return;
	glBindVertexArray(VAO);
	glUseProgram(program);
	GLuint l_M = glGetUniformLocation(program, "Model");
	glUniformMatrix4fv(l_M, 1, GL_FALSE, &Model[0][0]);
	GLuint l_c = glGetUniformLocation(program, "pColor");
	glUniform3fv(l_c, 1, &color[0]);
	GLuint l_a = glGetUniformLocation(program, "alive");
	glUniform1i(l_a, alive);
	/*
	if (!alive) {
		GLuint l_t = glGetUniformLocation(program, "t");
		glUniform1f(l_t, timeDead);
		GLuint l_s = glGetUniformLocation(program, "spin");
		glUniform3fv(l_s, 1, &spin[0]);
	}*/
	//glDrawArrays(GL_TRIANGLES, 0, 60);
	GLuint l_o = glGetUniformLocation(program, "outline");
	glUniform1i(l_o, true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glUniform1i(l_o, false);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Asteroid::move() {
	//vel -= 0.001f * pos / dot(pos, pos);
	if (HP < 0 && alive) explode();
	Polygon::move();
	if (!alive) {
		timeDead += 0.1;
		if (timeDead > 20.0) asteroids.erase(find(asteroids.begin(), asteroids.end(), this));
	}
}

void Asteroid::explode() {
	alive = false;
	//vel = vec3(0.0f);
	//Spin = mat3(1.0);
	if (pointCloud.size() > 20) {
		split();
	}
	ParticleCluster::particles.push_back(
		new ParticleCluster(400, pos, color, size)
	);
	
}

void Asteroid::loadGlowShader() {
	glowProgram = loadShaders("glowVert.shader", "glowFrag.shader");
	GLuint glowVBO;
	glGenBuffers(1, &glowVBO);
	const float vertices[] = {
		-1.0, 1.0,
		1.0, -1.0,
		-1.0, -1.0,
		1.0, -1.0,
		-1.0, 1.0,
		1.0, 1.0,
	};
	glBindBuffer(GL_ARRAY_BUFFER, glowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &glowVAO);
	glBindVertexArray(glowVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Asteroid::renderGlow(mat4 VP) {
	std::vector<mat4> MVPs;
	std::vector<vec3> colors;
	std::vector<float> sizes;
	std::vector<float> ts;
	for (auto a : asteroids) {
		ts.push_back(a->timeDead);
		MVPs.push_back(VP * a->Model);
		colors.push_back(a->color);
		sizes.push_back(a->size);
	}
	int count = MVPs.size();
	glUseProgram(glowProgram);
	glUniformMatrix4fv(glGetUniformLocation(glowProgram, "MVP"), count, GL_FALSE, &MVPs[0][0][0]);
	glUniform3fv(glGetUniformLocation(glowProgram, "color"), count, &colors[0][0]);
	glUniform1fv(glGetUniformLocation(glowProgram, "size"), count, &sizes[0]);
	glUniform1fv(glGetUniformLocation(glowProgram, "t"), count, &ts[0]);
	glUniform1i(glGetUniformLocation(glowProgram, "count"), count);
	glBindVertexArray(glowVAO);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}

bool Asteroid::detectCollision(Asteroid& a) {
	if (length(pos - a.pos) > size + a.size) return false;
	for (auto t : hull) {
		bool inside = true;
		for (int i = 0; i < 3; i++) {
			vec3 p0 = Model * vec4(t.pts[i], 1.0f);
			for (auto t2 : a.hull) {
				vec3 p1 = a.Model * vec4(t2.pts[0], 1.0f);
				if (dot(p0 - p1, t2.normal) > 0.0f) {
					inside = false;
					break;
				}
			}
			if (inside) return true;
		}
	}
	return false;
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
		float dv = length(p.vel - vel);
		Polygon::collide(p);
		p.decrementHP(size * dv * 2.0f);
		decrementHP(p.size * dv * 2.0f);
	}
}

void Asteroid::split() {
	vector<vec3> part0, part1;
	/*
	for (vec3 p : pointCloud) {
		//splits vertices into 8 by sign bit for x, y, and z
		uint bitmask = *(uint*)(&p[0]) >> 31;
		bitmask |= *(uint*)(&p[1]) >> 30 & 2;
		bitmask |= *(uint*)(&p[2]) >> 29 & 4;
		subparts[bitmask].push_back(p);
	}
	for (int i = 0; i < 8; i++) {
	asteroids.push_back(new Asteroid(this, subparts[i]));
	}
	*/
	//normal vector of random plane through origin
	vec3 normal = normalize(RNG::randomvec3() - vec3(0.5f));
	for (vec3 p : pointCloud) {
		if (dot(p, normal) < 0.0f) {
			part0.push_back(p);
		}
		else {
			part1.push_back(p);
		}
	}
	if (part0.size() > 4) {
		asteroids.push_back(new Asteroid(this, part0));
	}
	if (part1.size() > 4) {
		asteroids.push_back(new Asteroid(this, part1));
	}
}