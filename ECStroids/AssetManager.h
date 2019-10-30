#pragma once
#include "entt/entt.hpp"
#include "Event.h"

namespace AssetManager {
	void init(entt::registry* r, unsigned int* w, unsigned int* h);
	entt::entity createPlayer();
	entt::entity createBullet(entt::entity& shooter, bool tracking = false);
	entt::entity createAsteroid(entt::entity* parentAsteroid = nullptr);
	entt::entity createButton(Event::Type type);
	void clearScreen();
	void createMenu();
}