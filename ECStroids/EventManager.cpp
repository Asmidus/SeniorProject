#include "EventManager.h"
#include "Components.h"
#include "entt/entt.hpp"



EventManager::~EventManager() {}

void EventManager::registerEvent(Event event) {
	_events.push_back(event);
}

void EventManager::processEvents() {
	for (auto event : _events) {
		switch (event.type()) {
		case Event::Type::collision:
			processCollision(event);
		}
	}
	_events.clear();
}

void EventManager::processCollision(Event event) {
	//In the case of a collision, the first object is the collider, the second is the one being collided upon
	if (event.entities().size() >= 2) {
		auto collider = event.entities()[0];
		for (int i = 1; i < event.entities().size(); i++) {
			auto collided = event.entities()[i];
			_registry->get<Position>(collided).y += 1;
		}
		_registry->get<Position>(collider).x += 1;
	}
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