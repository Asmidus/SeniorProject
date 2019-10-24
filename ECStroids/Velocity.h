#pragma once
#include <glm/glm.hpp>

struct Velocity {
	Velocity(glm::vec2 dir) :
		direction(dir),
		maxSpeed(2),
		accel(1),
		decel(0.1),
		currVel(0, 0),
		currAccel(0) {}
	Velocity(glm::vec2 dir, float speed) :
		direction(dir),
		maxSpeed(speed),
		accel(1),
		decel(0),
		currAccel(0),
		currVel(glm::normalize(dir) * speed) {}
	glm::vec2 direction;
	glm::vec2 currVel;
	float maxSpeed;
	float currAccel;
	float accel;
	float decel;
};