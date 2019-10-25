#include "AssetManager.h"
#include "Event.h"
#include <SDL.h>
#include "AssetManager.h"
#include "Components.h"



AssetManager::~AssetManager() {}

entt::entity AssetManager::createPlayer() {
	auto entity = _registry->create();
	std::unordered_map<unsigned int, Event::Type> mouseMap;
	std::unordered_map<unsigned int, Event::Type> keyMap;
	std::unordered_map<Event::Type, unsigned int> cooldowns;
	mouseMap[SDL_BUTTON_LEFT] = Event::Type::shootBullet;
	cooldowns[Event::Type::shootBullet] = 20;
	keyMap[SDLK_w] = Event::Type::moveUp;
	keyMap[SDLK_s] = Event::Type::moveDown;
	keyMap[SDLK_d] = Event::Type::moveRight;
	keyMap[SDLK_a] = Event::Type::moveLeft;
	_registry->assign<Velocity>(entity, glm::vec2(1, 0));
	_registry->assign<MouseListener>(entity, mouseMap);
	_registry->assign<KeyListener>(entity, keyMap);
	_registry->assign<Sprite>(entity, "media/ECSplayer.png", 50, 50);
	_registry->assign<Transform>(entity, 0, 0, 25, 25);
	_registry->assign<Cooldown>(entity, cooldowns);
	return entity;
}

entt::entity AssetManager::createBullet(entt::entity& shooter, bool tracking) {
	auto entity = _registry->create();
	auto& shooterTransform = _registry->get<Transform>(shooter);
	static unsigned int bulletSize = 10;
	//TODO maybe find a better way to do this?
	glm::vec2 spawnPos = shooterTransform.pos;
	spawnPos.x += shooterTransform.rect.w / 2 - bulletSize / 2;
	spawnPos.y += shooterTransform.rect.h / 2 - bulletSize / 2;
	spawnPos += _registry->get<Velocity>(shooter).direction * glm::vec2(shooterTransform.rect.w / 2);
	_registry->assign<Velocity>(entity, _registry->get<Velocity>(shooter).direction, 10.0f);
	_registry->assign<Transform>(entity, spawnPos.x, spawnPos.y, bulletSize, bulletSize);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50);
	return entity;
}