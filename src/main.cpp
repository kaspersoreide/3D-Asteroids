#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <iostream>
#include "GL/glew.h"
#include "glm/ext.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"
#include "GLFW/glfw3.h"
#include <vector>
#include "glm/gtc/matrix_transform.hpp"

#include "random.h"
#include "particles.h"
#include "asteroid.h"
#include "player.h"
#include "skybox.h"
#include "framebuffer.h"

GLFWwindow* window;
Player* player;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E) {
		if (action == GLFW_PRESS) player->rot[0] = true;
		if (action == GLFW_RELEASE) player->rot[0] = false;
	}
	if (key == GLFW_KEY_Q) {
		if (action == GLFW_PRESS) player->rot[1] = true;
		if (action == GLFW_RELEASE) player->rot[1] = false;
	}
	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) player->rot[2] = true;
		if (action == GLFW_RELEASE) player->rot[2] = false;
	}
	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) player->rot[3] = true;
		if (action == GLFW_RELEASE) player->rot[3] = false;
	}
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) player->rot[4] = true;
		if (action == GLFW_RELEASE) player->rot[4] = false;
	}
	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) player->rot[5] = true;
		if (action == GLFW_RELEASE) player->rot[5] = false;
	}

	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) player->mov[0] = true;
		if (action == GLFW_RELEASE) player->mov[0] = false;
	}
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) player->mov[1] = true;
		if (action == GLFW_RELEASE) player->mov[1] = false;
	}
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) player->mov[2] = true;
		if (action == GLFW_RELEASE) player->mov[2] = false;
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) player->mov[3] = true;
		if (action == GLFW_RELEASE) player->mov[3] = false;
	}
	if (key == GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_PRESS) player->mov[4] = true;
		if (action == GLFW_RELEASE) player->mov[4] = false;
	}
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) player->mov[5] = true;
		if (action == GLFW_RELEASE) player->mov[5] = false;
	}
	if (key == GLFW_KEY_ESCAPE) {
		glfwTerminate();
	}
	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) player->shooting = true;
		if (action == GLFW_RELEASE) player->shooting = false;
	}
}

int main() {
	glfwInit();
	window = glfwCreateWindow(1280, 720, "3D Asteroids", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = true; // Needed for core profile
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2.0f);
	glPointSize(2.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glfwSetKeyCallback(window, keyCallback);

	RNG::srand(14027);
	Skybox::Init();
	ParticleCluster::loadPrograms();
	Polygon::loadProgram();
	Player::loadVertexArray();
	Asteroid::loadVertexArrays();
	Asteroid::loadGlowShader();
	Shooter::loadProgram();

	player = new Player();

	Framebuffer framebuffer;
	
	/*
	for (int i = 0; i < 15; i++) {
		Asteroid::asteroids.push_back(new Asteroid(
			100.0f * RNG::randomvec3() - 50.0f,
			2.0f + 3.0f * RNG::randomFloat()
		));
		Asteroid::asteroids[i]->setSpin(0.008f * RNG::randomvec3() - vec3(0.004f));
	}
	*/
	player->move();
	
	mat4 Projection = perspective(
		1.2f,
		static_cast<float>(16) / 9,
		0.1f,
		100.0f
	);
	/*
	mat4 View = lookAt(
		vec3(-12.0, 0.0, 0.0),
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0)
	);
	mat4 VP = Projection * View;
	Polygon::setViewProjection(VP);
	*/
	Polygon::setViewProjection(Projection * player->getView());
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mat4 VP = Projection * player->getView();
		Polygon::setViewProjection(VP);
		Polygon::setCameraPosition(player->getPos());
		ParticleCluster::setViewProjection(VP);
		Skybox::setViewProjection(player->getRotation(), Projection);
		Shooter::setVP(VP);
		Asteroid::spawn(player->getPos());
		//Asteroid::renderGlow(VP);
		
		Skybox::render();
		for (int i = 0; i < Asteroid::asteroids.size(); i++) {
			Asteroid* a1 = Asteroid::asteroids[i];
			//inner loop is a bit complicated,
			//because colliding i with j is the same as j with i
			if (a1->isAlive()) {
				for (int j = i + 1; j < Asteroid::asteroids.size(); j++) {
					Asteroid* a2 = Asteroid::asteroids[j];
					if (!a2->isAlive()) continue;
					//it->gravitate(**jt);
					a2->collide(*a1);
					//if (Asteroid::asteroids.find(a1) == nullptr) break;
				}
				//it->gravitate(player);
				player->collideShots(*a1);
				a1->collide(*player);
			}
			a1->move();
			a1->render();
			a1->cleanup(player->getPos());
		}
		for (auto p : ParticleCluster::particles) {
			p->doStuff();
		}
		player->move();
		player->render();

		framebuffer.renderTextureToScreen();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}