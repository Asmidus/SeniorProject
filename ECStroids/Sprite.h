#pragma once
#include "Transform.h"
#include "TextureManager.h"
#include "Vertex.h"

struct Sprite {
	Sprite() = default;

	Sprite(const char* path) {
		setSprite(path);
	}

	Sprite(const char* path, int w, int h, glm::vec3 color = { 255, 255, 255 }) {
		src.w = w;
		src.h = h;
		src.x = src.y = 0;
		this->color = color;
		setSprite(path);
	}

	void setSprite(const char* path) {
		auto [tex, w, h] = TextureManager::LoadTexture(path);
		texture = tex;
		size.x = w;
		size.y = h;
	}

	glm::vec4 getUV() {
		return glm::vec4(src.x / float(size.x), src.y / float(size.y), src.w / float(size.x), src.h / float(size.y));
	}

	//void setColor(int r, int g, int b) {
	//	if (r < 0) {
	//		r = 0;
	//	}
	//	if (g < 0) {
	//		g = 0;
	//	}
	//	if (b < 0) {
	//		b = 0;
	//	}
	//	this->r = r;
	//	this->g = g;
	//	this->b = b;
	//	SDL_SetTextureColorMod(, r, g, b);
	//}

	//void update() {
	//	if (animated) {
	//		src.x = src.w * static_cast<int>((SDL_GetTicks() / (int)delay) % frames);
	//	}
	//	if (scrolling) {
	//		transform->pos.x += delay;
	//		if (transform->pos.x + transform->w <= 0) {
	//			transform->pos.x = 0;
	//		}
	//		scrollDest.x = static_cast<int>(transform->pos.x + transform->w);
	//		scrollDest.y = static_cast<int>(transform->pos.y);
	//		scrollDest.w = transform->w * transform->scale;
	//		scrollDest.h = transform->h * transform->scale;
	//	}
	//	dest.x = static_cast<int>(transform->pos.x);
	//	dest.y = static_cast<int>(transform->pos.y);
	//	dest.w = transform->w * transform->scale;
	//	dest.h = transform->h * transform->scale;
	//	if (transform->angle) {
	//		rotated = true;
	//	}
	//}

	float angle = 0.0f;
	SDL_Rect src, scrollDest;
	glm::ivec2 size;
	Color color;
	unsigned int texture;
};