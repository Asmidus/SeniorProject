#pragma once
#include <SDL.h>
#include <unordered_map>

class TextureManager {
public:
	static void init(SDL_Renderer* r);
	static unsigned int LoadTexture(const char* fileName);
	static void Draw(unsigned int texIndex, SDL_Rect src, SDL_Rect dest);
	static void Draw(unsigned int texIndex, SDL_Rect src, SDL_Rect dest, SDL_Point* center, double angle);
};