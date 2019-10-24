#pragma once
#include <glm/glm.hpp>

struct Velocity {
	Velocity(float x, float y) :
		direction(1, 0),
		currSpeed(0),
		maxSpeed(10),
		accel(20),
		decel(20),
		currVel(0, 0),
		currAccel(0) {}
	glm::vec2 direction;
	glm::vec2 currVel;
	float currSpeed;
	float maxSpeed;
	float currAccel;
	float accel;
	float decel;
};