#pragma once
#include <SDL.h>

class TextureManager {
public:
	static void init(SDL_Renderer* r);
	static SDL_Texture* LoadTexture(const char* fileName);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_Point* center, double angle);
};