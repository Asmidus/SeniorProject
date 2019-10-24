#pragma once
#include <unordered_map>
#include "Event.h"

//A cooldown is a restriction on an entity that prevents it from triggering an event for the specified amount of time
//For example - the player entity not being able to shoot or collide with other entities 500 times a second
struct Cooldown {
	std::unordered_map<Event::Type, unsigned int> events;
	std::unordered_map<Event::Type, unsigned int> coolingDown;

	Cooldown(std::unordered_map<Event::Type, unsigned int> eventMap) {
		events = eventMap;
	}

	void decrementCooldowns() {
		for (auto it = coolingDown.begin(); it != coolingDown.end();) {
			it->second -= 1;
			if (it->second == 0) {
				coolingDown.erase(it++);
			} else {
				it++;
			}
		}
	}

	bool trigger(Event::Type type) {
		if (coolingDown.find(type) == coolingDown.end()) {
			coolingDown[type] = events[type];
			return true;
		}
		return false;
	}
};