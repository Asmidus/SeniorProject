#include "TextureManager.h"
#include <iostream>
#include <SDL_image.h>

static SDL_Renderer* _renderer = nullptr;
static std::unordered_map<const char*, unsigned int> _texIndices;
static std::vector<SDL_Texture*> _textures;

void TextureManager::init(SDL_Renderer* r) {
	_renderer = r;
}

unsigned int TextureManager::LoadTexture(const char* fileName) {
	if (_texIndices.find(fileName) == _texIndices.end()) {
		SDL_Surface* tempSurface = IMG_Load(fileName);
		SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, tempSurface);
		if (!tex) {
			std::cout << "OH NO" << std::endl;
		}
		SDL_FreeSurface(tempSurface);
		_texIndices[fileName] = _textures.size();
		_textures.push_back(tex);
	}
	return _texIndices[fileName];
}

void TextureManager::Draw(unsigned int texIndex, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(_renderer, _textures[texIndex], &src, &dest);
}

void TextureManager::Draw(unsigned int texIndex, SDL_Rect src, SDL_Rect dest, SDL_Point* center, double angle, glm::vec3 rgb) {
	SDL_SetTextureColorMod(_textures[texIndex], rgb.r, rgb.g, rgb.b);
	SDL_RenderCopyEx(_renderer, _textures[texIndex], &src, &dest, angle, center, SDL_FLIP_NONE);
}