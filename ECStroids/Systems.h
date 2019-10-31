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
	void init(float screenWidth, float screenHeight) { _screenWidth = screenWidth; _screenHeight = screenHeight; }
	void updateDelta(float dt) { _dt = dt; }
	void drawSprites();
	void updateAnimations();
	void moveEntities();
	void checkCollisions();
	void checkInput();
	void checkLifetimes();
	void spawnAsteroids();
private:
	float _dt;
	float _screenWidth, _screenHeight;
	entt::registry* _registry;
	EventManager* _events;
	InputManager* _inputs;
};

