#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <glm/glm.hpp>



namespace TextureManager {
	std::shared_ptr<sf::Texture> LoadTexture(const char* fileName);
}