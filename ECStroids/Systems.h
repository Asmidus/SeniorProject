#pragma once
#include "entt/entt.hpp"
#include <GL/glew.h>
#include "InputManager.h"

class EventManager;
class Systems
{
public:
	Systems(entt::registry* registry, EventManager* events, InputManager* inputs) : _registry(registry), _events(events), _inputs(inputs) {}
	~Systems() {}
	void drawSprites();
	void moveEntities();
	void checkInput();
private:
	entt::registry* _registry;
	EventManager* _events;
	InputManager* _inputs;
};

