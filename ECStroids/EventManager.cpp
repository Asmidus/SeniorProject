#include "EventManager.h"
#include "Components.h"
#include "AssetManager.h"
#include "entt/entt.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>



EventManager::~EventManager() {}

void EventManager::registerEvent(Event event) {
	_events.push_back(event);
}

int EventManager::processEvents(float dt) {
	_dt = dt;
	for (auto event : _events) {
		switch (event.type) {
		case Event::shootBullet:
			processShoot(event);
			break;
		case Event::Type::collision:
			processCollision(event);
			break;
		case Event::Type::moveUp:
		case Event::Type::moveRight:
		case Event::Type::moveLeft:
			processMove(event);
			break;
		case Event::Type::startGame:
			processStartGame(event);
			break;
		case Event::Type::quit:
			if (processQuit(event)) {
				return 1;
			}
			break;
		}
	}
	_events.clear();
	return 0;
}

void EventManager::processCollision(Event& event) {
	for (int i = 0; i < event.entities.size(); i++) {
		entt::entity collided = event.entities.at(i);
		if (!_registry->valid(collided)) {
			continue;
		}
		auto& collider = _registry->get<Collider>(collided);
		auto health = _registry->try_get<Health>(collided);
		if (!_registry->has<entt::tag<"Player"_hs>>(collided)) {
			if (_registry->has<entt::tag<"Split"_hs>>(collided)) {
				for (int i = 0; i < rand() % 2 + 2; i++) {
					AssetManager::createAsteroid(&collided);
				}
			}
			_registry->destroy(collided);
		} else {
			if (health) {
				health->current -= 1.0f;
				if (health->current > 0) {
					auto& sprite = _registry->get<Sprite>(collided);
					sprite.color = { 255, GLubyte(255 * health->current / health->max), GLubyte(255 * health->current / health->max), 255 };
					continue;
				}
				AssetManager::clearScreen();
				AssetManager::createMenu();
				return;
			}
			_registry->destroy(collided);
		}
	}
}

void EventManager::processMove(Event& event) {
	for (auto entity : event.entities) {
		auto& entityVel = _registry->get<Velocity>(entity);
		auto animation = _registry->try_get<Animation>(entity);
		auto& direction = entityVel.direction;
		auto& transform = _registry->get<Transform>(entity);
		switch (event.type) {
		case Event::moveUp:
			if (animation) {
				animation->active = true;
			}
			entityVel.currAccel = entityVel.accel;
			break;
		case Event::moveRight:
			direction = glm::rotate<float>(direction, 6*_dt);
			break;
		case Event::moveLeft:
			direction = glm::rotate<float>(direction, -6*_dt);
			break;
		}
		glm::normalize(direction);
		transform.angle = glm::angle(direction, glm::vec2(1, 0));
		//make sure we can get angles larger than 180 degrees
		if (direction.y < 0) transform.angle *= -1;
	}
}

void EventManager::processShoot(Event& event) {
	auto bullet = AssetManager::createBullet(event.entities[0]);
}

void EventManager::processStartGame(Event& event) {
	if (!event.entities.empty()) {
		auto rect = _registry->get<Transform>(event.entities[0]).rect;
		auto pos = event.mousePos;
		if (pos.x < rect.x || pos.x > rect.x + rect.w ||
			pos.y < rect.y || pos.y > rect.y + rect.h) {
			return;
		}
	}
	AssetManager::clearScreen();
	AssetManager::createPlayer();
	AssetManager::createAsteroidSpawner();
}

int EventManager::processQuit(Event& event) {
	if (!event.entities.empty()) {
		auto rect = _registry->get<Transform>(event.entities[0]).rect;
		auto pos = event.mousePos;
		if (pos.x < rect.x || pos.x > rect.x + rect.w ||
			pos.y < rect.y || pos.y > rect.y + rect.h) {
			return 0;
		}
	}
	return 1;
}