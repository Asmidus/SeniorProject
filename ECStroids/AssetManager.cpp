#include "AssetManager.h"
#include "Event.h"
#include <SDL.h>
#include "AssetManager.h"
#include "Components.h"
#include <iostream>



AssetManager::~AssetManager() {}

entt::entity AssetManager::createPlayer() {
	auto entity = _registry->create();
	std::unordered_map<unsigned int, Event::Type> mouseMap;
	std::unordered_map<unsigned int, Event::Type> keyMap;
	std::unordered_map<Event::Type, float> cooldowns;
	mouseMap[SDL_BUTTON_LEFT] = Event::Type::shootBullet;
	cooldowns[Event::Type::shootBullet] = 0.2f;
	keyMap[SDLK_w] = Event::Type::moveUp;
	keyMap[SDLK_s] = Event::Type::moveDown;
	keyMap[SDLK_d] = Event::Type::moveRight;
	keyMap[SDLK_a] = Event::Type::moveLeft;
	_registry->assign<Velocity>(entity, glm::vec2(1, 0), 2, 2);
	_registry->assign<MouseListener>(entity, mouseMap);
	_registry->assign<KeyListener>(entity, keyMap);
	_registry->assign<Sprite>(entity, "media/ECSplayer.png", 50, 50);
	_registry->assign<Transform>(entity, 0, 0, 50, 50, 15, 25);
	_registry->assign<Cooldown>(entity, cooldowns);
	_registry->assign<Animation>(entity, 5, 0.08, true);
	return entity;
}

entt::entity AssetManager::createBullet(entt::entity& shooter, bool tracking) {
	auto entity = _registry->create();
	auto& shooterTransform = _registry->get<Transform>(shooter);
	static unsigned int bulletSize = 10;
	glm::vec2 point = glm::vec2(shooterTransform.rect.w, shooterTransform.rect.h/2);
	float angle = (shooterTransform.angle) * (3.14159 / 180); // Convert to radians
	auto center = shooterTransform.center;
	float rotatedX = cos(angle) * (point.x - center.x) - sin(angle) * (point.y - center.y) + center.x + shooterTransform.rect.x - bulletSize/2;
	float rotatedY = sin(angle) * (point.x - center.x) + cos(angle) * (point.y - center.y) + center.y + shooterTransform.rect.y - bulletSize/2;
	_registry->assign<Velocity>(entity, _registry->get<Velocity>(shooter).direction, 5.0f);
	_registry->assign<Transform>(entity, rotatedX, rotatedY, bulletSize, bulletSize);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50);
	return entity;
}