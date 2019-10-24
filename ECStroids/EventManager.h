#pragma once
#include "Event.h"
#include "AssetManager.h"
#include "entt/entt.hpp"

class EventManager {
public:
	EventManager(entt::registry* registry, AssetManager* assets) : _registry(registry), _assets(assets) {}
	~EventManager();
	void registerEvent(Event event);
	void processEvents();
	void processCollision(Event event);
	void processButton(Event event);
	void processMove(Event event);
	void processShoot(Event event);
private:
	std::vector<Event> _events;
	AssetManager* _assets;
	entt::registry* _registry;
};

