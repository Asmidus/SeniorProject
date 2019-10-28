#include "EventManager.h"
#include "Components.h"
#include "AssetManager.h"
#include "entt/entt.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>



EventManager::~EventManager() {}

void EventManager::registerEvent(Event event) {
	_events.push_back(event);
}

void EventManager::processEvents(float dt) {
	_dt = dt;
	for (auto event : _events) {
		switch (event.type()) {
		case Event::shootBullet:
			processShoot(event);
			break;
		case Event::Type::collision:
			processCollision(event);
			break;
		case Event::Type::moveUp:
		case Event::Type::moveDown:
		case Event::Type::moveRight:
		case Event::Type::moveLeft:
			processMove(event);
			break;
		}
	}
	_events.clear();
}

void EventManager::processCollision(Event event) {
	for (int i = 0; i < event.entities().size(); i++) {
		entt::entity collided = event.entities().at(i);
		if (!_registry->valid(collided)) {
			continue;
		}
		auto& collider = _registry->get<Collider>(collided);
		auto health = _registry->try_get<Health>(collided);
		if (!_registry->has<entt::tag<"Player"_hs>>(collided)) {
			if (_registry->has<entt::tag<"Split"_hs>>(collided)) {
				for (int i = 0; i < rand() % 2 + 2; i++) {
					_assets->createAsteroid(collided);
				}
			}
			_registry->destroy(collided);
		} else {
			if (health) {
				health->current -= 1.0f;
				if (health->current > 0) {
					auto& sprite = _registry->get<Sprite>(collided);
					sprite.color = { 255, 255 * health->current / health->max, 255 * health->current / health->max };
					continue;
				}
			}
			_registry->destroy(collided);
		}
	}
}

void EventManager::processButton(Event event) {
	//for (auto entity : event.entities()) {
	//	std::string tag = _registry->get<Data>(entity).tag;
	//	if (tag == "easy") {
	//		//start game on easy mode
	//	} else if (tag == "hard") {
	//		//start game on hard mode
	//	}
	//}
}

void EventManager::processMove(Event event) {
	for (auto entity : event.entities()) {
		auto& entityVel = _registry->get<Velocity>(entity);
		auto animation = _registry->try_get<Animation>(entity);
		auto& direction = entityVel.direction;
		switch (event.type()) {
		case Event::moveUp:
			//direction.y += 1;
			if (animation) {
				animation->active = true;
			}
			entityVel.currAccel = entityVel.accel;
			break;
		case Event::moveDown:
			//The player can't move backwards
			//direction.y -= 1;
			break;
		case Event::moveRight:
			direction = glm::rotate<float>(direction, 6*_dt);
			//direction.x += 1;
			break;
		case Event::moveLeft:
			//direction.x -= 1;
			direction = glm::rotate<float>(direction, -6*_dt);
			break;
		}
		glm::normalize(entityVel.direction);
	}
}

void EventManager::processShoot(Event event) {
	auto bullet = _assets->createBullet(event.entities()[0]);
	//_registry->get<Transform>(bullet).pos = _registry->get<Transform>(event.entities()[0]).pos;
	//_registry->get<Velocity>(bullet).direction = _registry->get<Velocity>(event.entities()[0]).direction;
}