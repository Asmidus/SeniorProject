#pragma once
#include <SDL.h>
#include <glm/glm.hpp>

struct Transform {
	glm::vec2 pos;
	float angle;
	SDL_Rect rect;
	SDL_Point center;

	Transform() {
		center.x = 25;
		center.y = 25;
		rect.x = pos.y = 0;
		rect.y = pos.x = 0;
		rect.w = 50;
		rect.h = 50;
		angle = 0.0f;
	}

	Transform(float x, float y) : Transform() {
		rect.x = pos.x = x;
		rect.y = pos.y = y;
	}
	
	Transform(float x, float y, float w, float h) : Transform(x, y) {
		rect.w = w;
		rect.h = h;
		center.x = w / 2;
		center.y = h / 2;
	}

	Transform(const Transform& other, float w, float h) : Transform(other.rect.x, other.rect.y, w, h) {
		angle = other.angle;
	}

	void updatePos(glm::vec2 delta) {
		pos += delta;
		rect.x = pos.x;
		rect.y = pos.y;
	}
};