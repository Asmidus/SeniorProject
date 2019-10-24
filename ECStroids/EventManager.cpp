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

void EventManager::processEvents() {
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
	//In the case of a collision, the first object is the collider, the second is the one being collided upon
	//if (event.entities().size() >= 2) {
	auto collider = event.entities()[0];
	for (int i = 1; i < event.entities().size(); i++) {
		auto collided = event.entities()[i];
		_registry->get<Transform>(collided).pos.y += 1;
	}
	_registry->get<Transform>(collider).pos.x += 1;
	//}
}

void EventManager::processButton(Event event) {
	for (auto entity : event.entities()) {
		std::string tag = _registry->get<Data>(entity).tag;
		if (tag == "easy") {
			//start game on easy mode
		} else if (tag == "hard") {
			//start game on hard mode
		}
	}
}

void EventManager::processMove(Event event) {
	for (auto entity : event.entities()) {
		auto& entityVel = _registry->get<Velocity>(entity);
		auto& direction = entityVel.direction;
		switch (event.type()) {
		case Event::moveUp:
			//direction.y += 1;
			entityVel.currAccel = entityVel.accel;
			break;
		case Event::moveDown:
			//The player can't move backwards
			//direction.y -= 1;
			break;
		case Event::moveRight:
			direction = glm::rotate<float>(direction, 0.05);
			//direction.x += 1;
			break;
		case Event::moveLeft:
			//direction.x -= 1;
			direction = glm::rotate<float>(direction, -0.05);
			break;
		}
		glm::normalize(entityVel.direction);
		entityVel.direction = direction;
	}
}

void EventManager::processShoot(Event event) {
	auto bullet = _assets->createBullet(true);
	_registry->get<Transform>(bullet) = _registry->get<Transform>(event.entities()[0]);
}