#pragma once
#include "entt/entt.hpp"
#include <GL/glew.h>
#include "InputManager.h"

class EventManager;
class SpriteBatch;
class Camera;
class Program;
class Systems
{
public:
	Systems(entt::registry* registry, EventManager* events, InputManager* inputs) : _registry(registry), _events(events), _inputs(inputs) {}
	~Systems() {}
	void init(Program* program, float screenWidth, float screenHeight) { _gameWidth = screenWidth; _gameHeight = screenHeight; _program = program; }
	void updateDelta(float dt) { _dt = dt; }
	void drawSprites(SpriteBatch* batch);
	void updateAnimations();
	void moveEntities();
	void checkCollisions();
	void checkInput(Camera* currCam);
	void checkLifetimes();
	void spawnAsteroids();
private:
	float _dt;
	float _gameWidth, _gameHeight;
	entt::registry* _registry;
	EventManager* _events;
	InputManager* _inputs;
	Program* _program;
};

