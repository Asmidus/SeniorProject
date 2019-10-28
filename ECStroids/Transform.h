#pragma once
#include <SDL.h>
#include <glm/glm.hpp>

struct Transform {
	float angle;
	SDL_FRect rect;
	SDL_FPoint center;

	Transform() {
		center.x = 25;
		center.y = 25;
		rect.x = 0;
		rect.y = 0;
		rect.w = 50;
		rect.h = 50;
		angle = 0.0f;
	}

	Transform(float x, float y) : Transform() {
		rect.x = x;
		rect.y = y;
	}
	
	Transform(float x, float y, float w, float h) : Transform(x, y) {
		rect.w = w;
		rect.h = h;
		center.x = w / 2;
		center.y = h / 2;
	}

	Transform(float x, float y, float w, float h, float cx, float cy) : Transform(x, y, w, h) {
		center.x = cx;
		center.y = cy;
	}

	Transform(const Transform& other, float w, float h) : Transform(other.rect.x, other.rect.y, w, h) {
		angle = other.angle;
	}

	void updatePos(glm::vec2 delta) {
		rect.x += delta.x;
		rect.y += delta.y;
	}
};