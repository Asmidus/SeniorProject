#pragma once
#include "Event.h"
#include "entt/entt.hpp"
class EventManager
{
public:
	EventManager(entt::registry* registry) : _registry(registry) {}
	~EventManager();
	void registerEvent(Event event);
	void processEvents();
	void processCollision(Event event);
	void processButton(Event event);
private:
	std::vector<Event> _events;
	entt::registry* _registry;
};

