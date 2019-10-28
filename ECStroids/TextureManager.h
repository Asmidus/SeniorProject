#pragma once
#include <SDL.h>
#include <unordered_map>
#include <glm/glm.hpp>

class TextureManager {
public:
	static void init(SDL_Renderer* r);
	static unsigned int LoadTexture(const char* fileName);
	static void Draw(unsigned int texIndex, SDL_Rect src, SDL_Rect dest);
	static void Draw(unsigned int texIndex, SDL_Rect src, SDL_FRect dest, SDL_FPoint* center, double angle, glm::vec3 rgb = { 255, 255, 255 });
};