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
	mouseMap[SDL_BUTTON_LEFT] = Event::Type::shootBullet;
	keyMap[SDLK_w] = Event::Type::moveUp;
	keyMap[SDLK_s] = Event::Type::moveDown;
	keyMap[SDLK_d] = Event::Type::moveRight;
	keyMap[SDLK_a] = Event::Type::moveLeft;
	_registry->assign<Velocity>(entity, 1.0f, 0.0f);
	_registry->assign<MouseListener>(entity, mouseMap);
	_registry->assign<KeyListener>(entity, keyMap);
	_registry->assign<Sprite>(entity, "media/PlayerShip.png", 50, 50);
	_registry->assign<Transform>(entity);
	return entity;
}

entt::entity AssetManager::createBullet(bool friendly, bool tracking) {
	auto entity = _registry->create();
	//_registry->assign<Velocity>(entity, 10.0f, 0.0f);
	//_registry->assign<Position>(entity, 0.0f, 0.0f);
	//_registry->assign<Sprite>(entity, "media/Projectile.png", 20, 20);
	return entity;
}