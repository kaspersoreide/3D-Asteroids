#include "Player.h"
GLuint Player::VAO;

void Player::loadVertexArray() {
	VAO = loadObjectNormalized("tetrahedron.txt");
}

Player::Player() : Polygon(vec3(0.0f), 1.0f) {
	color = vec3(0.0, 1.0, 0.4);
	shooter = new Shooter();
}

void Player::render() {
	shooter->render();
	//Polygon::render(VAO);
}

void Player::move() {
	const float rotAmount = 0.0008f;
	const float movAmount = 0.0087f;
	vec3 right = Rotation * vec3(1.0, 0.0, 0.0);
	vec3 up = Rotation * vec3(0.0, 1.0, 0.0);
	vec3 forward = Rotation * vec3(0.0, 0.0, 1.0);
	vec3 dSpin(0.0f);
	if (rot[0]) dSpin += rotAmount * forward;
	if (rot[1]) dSpin -= rotAmount * forward;
	if (rot[2]) dSpin -= rotAmount * up;
	if (rot[3]) dSpin += rotAmount * up;
	if (rot[4]) dSpin -= rotAmount * right;
	if (rot[5]) dSpin += rotAmount * right;

	if (mov[0]) vel -= movAmount * forward;
	if (mov[1]) vel += movAmount * forward;
	if (mov[2]) vel -= movAmount * right;
	if (mov[3]) vel += movAmount * right;
	if (mov[4]) vel += movAmount * up;
	if (mov[5]) vel -= movAmount * up;
	/*
	if (dot(pos, pos) > 1000.0f) {
		vel -= 0.00002f * pos;
	}
	*/
	vel *= 0.99f;
	spin *= 0.99f;
	setSpin(spin + dSpin);
	shooter->setPosAndDir(pos, -forward);
	shooter->moveShots();
	shooter->setVel(vel);
	if (shooting) shoot();
	Polygon::move();
}

mat4 Player::getView() {
	//mat4 CamModel = Model;
	//CamModel[3] += Model * vec4(0.0, 1.0f, 4.0f, 0.0) - vec4(0.2f * vel, 0.0);
	return inverse(Model);
}

void Player::shoot() {
	shooter->shoot();
}