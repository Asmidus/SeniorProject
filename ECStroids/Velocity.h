#pragma once
#include <glm/glm.hpp>

struct Velocity {
	Velocity(glm::vec2 dir, float acceleration, float max) :
		direction(dir),
		maxSpeed(max),
		accel(acceleration),
		decel(acceleration/10),
		currVel(0, 0),
		currAccel(0),
		constant(false) {}
	Velocity(glm::vec2 dir, float speed) :
		direction(dir),
		maxSpeed(speed),
		accel(1),
		decel(0),
		currAccel(0),
		currVel(glm::normalize(dir) * speed),
		constant(true) {}
	glm::vec2 direction;
	glm::vec2 currVel;
	float maxSpeed;
	float currAccel;
	float accel;
	float decel;
	bool constant;
};