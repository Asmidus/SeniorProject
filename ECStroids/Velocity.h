#pragma once
#include <glm/glm.hpp>

struct Velocity {
	Velocity(float x, float y) :
		direction(1, 0),
		currSpeed(0),
		maxSpeed(2),
		accel(1),
		decel(0.1),
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