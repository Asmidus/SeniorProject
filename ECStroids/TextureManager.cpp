#include "TextureManager.h"
#include <iostream>
#include <SDL_image.h>

static SDL_Renderer* _renderer = nullptr;

void TextureManager::init(SDL_Renderer* r) {
	_renderer = r;
}

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(_renderer, tex, &src, &dest);
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_Point* center, double angle) {
	SDL_RenderCopyEx(_renderer, tex, &src, &dest, angle, center, SDL_FLIP_NONE);
}