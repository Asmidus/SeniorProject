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
	void updateDelta(float dt) { _dt = dt; }
	void drawSprites();
	void updateAnimations();
	void moveEntities();
	void checkInput();
private:
	float _dt;
	entt::registry* _registry;
	EventManager* _events;
	InputManager* _inputs;
};

