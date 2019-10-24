#pragma once
#include <SDL.h>
#include <glm/glm.hpp>

struct Transform {
	glm::vec2 pos;
	float angle;
	SDL_Point center;

	Transform() {
		center.x = 25;
		center.y = 25;
		pos.x = 0;
		pos.y = 0;
		angle = 0.0f;
	}

	Transform(float x, float y) {
		Transform();
		pos.x = x;
		pos.y = y;
	}
};