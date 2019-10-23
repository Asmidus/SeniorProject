#pragma once
#include "entt/entt.hpp"
#include <GL/glew.h>
#include "InputManager.h"

class EventManager;
class Systems
{
public:
	Systems(entt::registry* registry, EventManager* events, InputManager* inputs) : xOffset(0), yOffset(0), _registry(registry), _events(events), _inputs(inputs) {}
	~Systems() {}
	void init(GLint x, GLint y);
	void drawSprites();
	void moveEntities();
	void checkInput();
private:
	GLint xOffset;
	GLint yOffset;
	entt::registry* _registry;
	EventManager* _events;
	InputManager* _inputs;
};

