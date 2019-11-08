#pragma once
#include <SDL.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL_ttf.h>
#include <GL/glew.h>

namespace TextureManager {
	void init(GLuint tex);
	unsigned int LoadTexture(const char* fileName);
	SDL_Texture* LoadText(const char* text, TTF_Font* font, SDL_Color color);
	void Draw(unsigned int texture, SDL_Rect src, SDL_Rect dest);
	void DrawText(SDL_Texture* texture, SDL_Rect dest);
	void Draw(unsigned int texture, SDL_Rect src, SDL_FRect dest, SDL_FPoint* center, double angle, glm::vec3 rgb = { 255, 255, 255 });
}