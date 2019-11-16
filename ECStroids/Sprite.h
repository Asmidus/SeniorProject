#pragma once
#include "Transform.h"
#include "TextureManager.h"

struct Sprite : public sf::Sprite {

	Sprite(const char* path, int w, int h, glm::vec3 color = { 255, 255, 255 }, int x = 0, int y = 0) {
		this->setTexture(*TextureManager::LoadTexture(path));
		this->setTextureRect(sf::IntRect(0, 0, w, h));
		this->setColor(sf::Color(color.x, color.y, color.z, 255));
		this->setOrigin(w / 2, h / 2);
		this->setPosition(x, y);
	}
	Sprite(const char* path, sf::IntRect spriteDim, sf::FloatRect transDim, sf::Color color = sf::Color::White, unsigned int zLevel = 0) {
		this->setTexture(*TextureManager::LoadTexture(path));
		this->setTextureRect(spriteDim);
		this->setColor(color);
		this->setOrigin(spriteDim.width / 2.0f, spriteDim.height / 2.0f);
		this->setScale(transDim.width / spriteDim.width, transDim.height / spriteDim.height);
		this->setPosition(transDim.left - transDim.width / 2, transDim.top - transDim.height / 2);
		z = zLevel;
	}

	unsigned int z;
//	Sprite() = default;
//
//	Sprite(const char* path) {
//		setSprite(path);
//	}
//
//	Sprite(const char* path, int w, int h, glm::vec3 color = { 255, 255, 255 }) {
//		src.w = w;
//		src.h = h;
//		src.x = src.y = 0;
//		this->color = color;
//		setSprite(path);
//	}
//
//	void setSprite(const char* path) {
//		auto [tex, w, h] = TextureManager::LoadTexture(path);
//		texture = tex;
//		size.x = w;
//		size.y = h;
//	}
//
//	glm::vec4 getUV() {
//		return glm::vec4(src.x / float(size.x), src.y / float(size.y), src.w / float(size.x), src.h / float(size.y));
//	}
//
//	SDL_Rect src, scrollDest;
//	glm::ivec2 size;
//	Color color;
//	unsigned int texture;
};