#pragma once
#include <SDL.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL_ttf.h>
#include <GL/glew.h>



namespace TextureManager {
	std::tuple<unsigned int, unsigned int, unsigned int> LoadTexture(const char* fileName);
	SDL_Texture* LoadText(const char* text, TTF_Font* font, SDL_Color color);
}