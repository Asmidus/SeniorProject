#include "AssetManager.h"
#include <SDL.h>
#include "AssetManager.h"
#include "Components.h"
#include <iostream>



static entt::registry* _registry = nullptr;
static unsigned int* _screenWidth;
static unsigned int* _screenHeight;

void AssetManager::init(entt::registry* r, unsigned int* w, unsigned int* h) {
	_registry = r;
	_screenWidth = w;
	_screenHeight = h;
}

entt::entity AssetManager::createPlayer() {
	auto entity = _registry->create();
	static unsigned int shipSize = 30;
	std::unordered_map<unsigned int, Event::Type> keyMap;
	std::unordered_map<Event::Type, float> cooldowns;
	cooldowns[Event::Type::shootBullet] = 0.2f;
	cooldowns[Event::Type::collision] = 1.5f;
	keyMap[SDLK_w] = Event::Type::moveUp;
	keyMap[SDLK_d] = Event::Type::moveRight;
	keyMap[SDLK_a] = Event::Type::moveLeft;
	keyMap[SDLK_SPACE] = Event::Type::shootBullet;
	_registry->assign<Velocity>(entity, glm::vec2(1, 0), 2, 2);
	_registry->assign<KeyListener>(entity, keyMap);
	_registry->assign<Sprite>(entity, "media/ECSplayer.png", 50, 50);
	_registry->assign<Transform>(entity,
								 *_screenWidth/2 - shipSize/2, *_screenHeight/2 - shipSize/2,	//pos
								 shipSize, shipSize,											//size
								 shipSize / 2, shipSize / 2);									//center
	_registry->assign<Cooldown>(entity, cooldowns);
	_registry->assign<Animation>(entity, 5, 0.08, true);
	_registry->assign<Health>(entity, 5.0f);
	_registry->assign<entt::tag<"Screenwrap"_hs>>(entity);
	_registry->assign<Collider>(entity, shipSize/3);
	_registry->assign<entt::tag<"Player"_hs>>(entity);
	return entity;
}

entt::entity AssetManager::createBullet(entt::entity& shooter, bool tracking) {
	auto entity = _registry->create();
	auto& shooterTransform = _registry->get<Transform>(shooter);
	static unsigned int bulletSize = 12.5;
	glm::vec2 point = glm::vec2(shooterTransform.rect.w, shooterTransform.rect.h/2);
	float angle = (shooterTransform.angle) * (3.14159 / 180); // Convert to radians
	auto center = shooterTransform.center;
	float rotatedX = cos(angle) * (point.x - center.x) - sin(angle) * (point.y - center.y) + center.x + shooterTransform.rect.x - bulletSize/2;
	float rotatedY = sin(angle) * (point.x - center.x) + cos(angle) * (point.y - center.y) + center.y + shooterTransform.rect.y - bulletSize/2;
	_registry->assign<Velocity>(entity, _registry->get<Velocity>(shooter).direction, 5.0f);
	_registry->assign<Transform>(entity, rotatedX, rotatedY, bulletSize, bulletSize);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50, glm::vec3(0, 255, 0));
	_registry->assign<Lifetime>(entity, 0.75);
	_registry->assign<Collider>(entity, bulletSize/2);
	if (_registry->has<entt::tag<"Player"_hs>>(shooter)) {
		_registry->assign<entt::tag<"Player"_hs>>(entity);
	} else {
		_registry->assign<entt::tag<"Enemy"_hs>>(entity);
	}
	return entity;
}

entt::entity AssetManager::createAsteroid(glm::vec2 speedRange, glm::vec2 sizeRange) {
	auto entity = _registry->create();
	int xDir = 0, yDir = 0;
	while (!xDir || !yDir) {
		xDir = rand() % 200 - 100;
		yDir = rand() % 200 - 100;
	}
	bool topSpawn = rand() % 2;
	float x, y;
	if (topSpawn) {
		x = rand() % *_screenWidth;
		y = -99 + rand() % 2 * (*_screenHeight + 99);
	} else {
		x = -99 + rand() % 2 * (*_screenWidth + 99);
		y = rand() % *_screenHeight;
	}
	float speed = speedRange.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (speedRange.y - speedRange.x)));
	float size = sizeRange.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (sizeRange.y - sizeRange.x)));
	_registry->assign<Velocity>(entity, glm::vec2(xDir, yDir), speed);
	_registry->assign<Transform>(entity, x, y, size, size);
	_registry->assign<entt::tag<"Split"_hs>>(entity);
	_registry->assign<Collider>(entity, size / 2);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50, glm::vec3(150, 75, 0));
	_registry->assign<entt::tag<"Screenwrap"_hs>>(entity);
	_registry->assign<entt::tag<"Enemy"_hs>>(entity);
	return entity;
}

entt::entity AssetManager::createAsteroid(entt::entity* parentAsteroid) {
	auto entity = _registry->create();
	int xDir = 0, yDir = 0;
	while (!xDir || !yDir) {
		xDir = rand() % 200 - 100;
		yDir = rand() % 200 - 100;
	}
	auto transform = _registry->get<Transform>(*parentAsteroid);
	float speed = _registry->get<Velocity>(*parentAsteroid).maxSpeed + 0.5f;
	_registry->assign<Velocity>(entity, glm::vec2(xDir, yDir), speed);
	_registry->assign<Transform>(entity, transform.rect.x, transform.rect.y, transform.rect.w / 2, transform.rect.h / 2);
	if (transform.rect.w > 75) {
		_registry->assign<entt::tag<"Split"_hs>>(entity);
	}
	_registry->assign<Collider>(entity, _registry->get<Collider>(*parentAsteroid).radius/2);
	_registry->assign<Sprite>(entity, "media/Projectile.png", 50, 50, glm::vec3(150, 75, 0));
	_registry->assign<entt::tag<"Screenwrap"_hs>>(entity);
	_registry->assign<entt::tag<"Enemy"_hs>>(entity);
	return entity;
}

entt::entity AssetManager::createButton(Event::Type type, const char* text) {
	auto entity = _registry->create();
	std::unordered_map<unsigned int, Event::Type> mouseMap;
	mouseMap[SDL_BUTTON_LEFT] = type;
	_registry->assign<Sprite>(entity, "media/Button.png", 160, 100, glm::vec3(255, 100, 100));
	_registry->assign<Transform>(entity, 0, 0, 160, 100);
	_registry->assign<MouseListener>(entity, mouseMap);
	_registry->assign<Text>(entity, text, 160, 100, 24, SDL_Color({ 25, 25, 25, 255 }));
	return entity;
}

entt::entity AssetManager::createAsteroidSpawner() {
	auto entity = _registry->create();
	_registry->assign<AsteroidSpawner>(entity, 3, 2, glm::vec2(0.25, 1), glm::vec2(75, 150));
	return entity;
}

void AssetManager::clearScreen() {
	_registry->each([](auto entity) {
		_registry->destroy(entity);
	});
}

void AssetManager::createMenu() {
	auto start = createButton(Event::Type::startGame, "START");
	auto& startRect = _registry->get<Transform>(start).rect;
	startRect.x = *_screenWidth / 4 - startRect.w / 2;
	startRect.y = *_screenHeight / 2 - startRect.h / 2;
	auto quit = createButton(Event::Type::quit, "QUIT");
	auto& quitRect = _registry->get<Transform>(quit).rect;
	quitRect.x = *_screenWidth * 3 / 4 - quitRect.w / 2;
	quitRect.y = *_screenHeight / 2 - quitRect.h / 2;
}