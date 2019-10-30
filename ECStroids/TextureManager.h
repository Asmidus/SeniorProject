#pragma once
#include <SDL.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace TextureManager {
	void init(SDL_Renderer* r);
	unsigned int LoadTexture(const char* fileName);
	void Draw(unsigned int texIndex, SDL_Rect src, SDL_Rect dest);
	void Draw(unsigned int texIndex, SDL_Rect src, SDL_FRect dest, SDL_FPoint* center, double angle, glm::vec3 rgb = { 255, 255, 255 });
}