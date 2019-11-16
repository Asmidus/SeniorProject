#include "TextureManager.h"
#include <iostream>



static std::unordered_map<const char*, std::shared_ptr<sf::Texture>> _textures;

std::shared_ptr<sf::Texture> TextureManager::LoadTexture(const char* fileName) {
	if (_textures.find(fileName) == _textures.end()) {
		auto texture = std::make_shared<sf::Texture>();
		texture->loadFromFile(fileName);
		_textures[fileName] = texture;
	}
	return _textures[fileName];
}