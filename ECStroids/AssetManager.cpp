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
	cooldowns[Event::Type::shootBullet] = 0.2f;
	cooldowns[Event::Type::collision] = 1.5f;
	keyMap[SDLK_w] = Event::Type::moveUp;
	keyMap[SDLK_s] = Event::Type::moveDown;
	keyMap[SDLK_d] = Event::Type::moveRight;
	keyMap[SDLK_a] = Event::Type::moveLeft;
	keyMap[SDLK_SPACE] = Event::Type::shootBullet;
	_registry->assign<Velocity>(entity, glm::vec2(1, 0), 2, 2);
	_registry->assign<KeyListener>(entity, keyMap);
	_registry->assign<Sprite>(entity, "media/ECSplayer.png", 50, 50);
	_registry->assign<Transform>(entity, 0, 0, 50, 50, 25, 25);
	_registry->assign<Cooldown>(entity, cooldowns);
	_registry->assign<Animation>(entity, 5, 0.08, true);
	_registry->assign<Health>(entity, 5.0f);
	_registry->assign<entt::tag<"Screenwrap"_hs>>(entity);
	_registry->assign<Collider>(entity, 15);
	_registry->assign<entt::tag<"Player"_hs>>(entity);
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
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50, glm::vec3(0, 255, 0));
	_registry->assign<Lifetime>(entity, 1.5);
	_registry->assign<Collider>(entity, 5);
	if (_registry->has<entt::tag<"Player"_hs>>(shooter)) {
		_registry->assign<entt::tag<"Player"_hs>>(entity);
	} else {
		_registry->assign<entt::tag<"Enemy"_hs>>(entity);
	}


	return entity;
}

entt::entity AssetManager::createAsteroid(float x, float y) {
	auto entity = _registry->create();
	float speed = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));
	int xDir = 0, yDir = 0;
	while (!xDir || !yDir) {
		xDir = rand() % 200 - 100;
		yDir = rand() % 200 - 100;
	}
	_registry->assign<Velocity>(entity, glm::vec2(xDir, yDir), speed);
	_registry->assign<Transform>(entity, x, y, 100, 100);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50, glm::vec3(150, 75, 0));
	_registry->assign<entt::tag<"Split"_hs>>(entity);
	_registry->assign<Collider>(entity, 40);
	_registry->assign<entt::tag<"Enemy"_hs>>(entity);
	return entity;
}

entt::entity AssetManager::createAsteroid(entt::entity& parentAsteroid) {
	auto entity = _registry->create();
	auto transform = _registry->get<Transform>(parentAsteroid);
	float speed = _registry->get<Velocity>(parentAsteroid).maxSpeed + 0.5f;
	int xDir = 0, yDir = 0;
	while (!xDir || !yDir) {
		xDir = rand() % 200 - 100;
		yDir = rand() % 200 - 100;
	}
	_registry->assign<Velocity>(entity, glm::vec2(xDir, yDir), speed);
	_registry->assign<Transform>(entity, transform.rect.x, transform.rect.y, transform.rect.w/2, transform.rect.h/2);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50, glm::vec3(150, 75, 0));
	_registry->assign<entt::tag<"Screenwrap"_hs>>(entity);
	if (transform.rect.w > 50) {
		_registry->assign<entt::tag<"Split"_hs>>(entity);
	}
	_registry->assign<Collider>(entity, _registry->get<Collider>(parentAsteroid).radius/2);
	_registry->assign<entt::tag<"Enemy"_hs>>(entity);
	return entity;
}

void AssetManager::clearScreen() {
	_registry->empty();
}

void AssetManager::createMenu() {
	//create buttons and stuff
}