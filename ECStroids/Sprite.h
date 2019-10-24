#pragma once
#include "Transform.h"
#include "TextureManager.h"

class Sprite {
public:
	int r, g, b;

	Sprite() = default;

	Sprite(const char* path) {
		setSprite(path);
	}

	Sprite(const char* path, int w, int h) {
		src.w = w;
		src.h = h;
		src.x = src.y = 0;
		dest.w = w;
		dest.h = h;
		setSprite(path);
	}

	Sprite(const char* path, int w, int h, int nFrames, int mSpeed) {
		animated = true;
		frames = nFrames;
		delay = mSpeed;
		src.w = w;
		src.h = h;
		dest.w = w;
		dest.h = h;
		setSprite(path);
	}

	//Sprite(MainGame* game, const char* path, int w, int h, double speed) {
	//	scrolling = true;
	//	delay = game->width/(1920/speed);
	//	src.w = w;
	//	src.h = h;
	//	setSprite(path);
	//}

	~Sprite() {
		SDL_DestroyTexture(texture);
	}

	void setSprite(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void setColor(int r, int g, int b) {
		if (r < 0) {
			r = 0;
		}
		if (g < 0) {
			g = 0;
		}
		if (b < 0) {
			b = 0;
		}
		this->r = r;
		this->g = g;
		this->b = b;
		SDL_SetTextureColorMod(texture, r, g, b);
	}

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

	//void draw() override {
	//	if (!rotated) {
	//		TextureManager::Draw(Game::renderer, texture, src, dest);
	//		if (scrolling) {
	//			TextureManager::Draw(Game::renderer, texture, src, scrollDest);
	//		}
	//	} else {
	//		TextureManager::Draw(Game::renderer, texture, src, dest, transform->center, transform->angle);
	//	}
	//}

	bool animated = false;
	bool scrolling = false;
	float angle = 0.0f;
	int frames = 0;
	double delay = 100;
	SDL_Rect src, dest, scrollDest;
	SDL_Texture* texture;
};