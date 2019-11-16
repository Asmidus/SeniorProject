#pragma once
#include "entt/entt.hpp"
#include "InputManager.h"
#include "SpriteBatch.h"
#include <SFML/Graphics.hpp>

class EventManager;
class SpriteBatch;
class Camera;
class Program;
class Systems
{
public:
	Systems(entt::registry* registry, sf::RenderWindow* window, EventManager* events, InputManager* inputs) :
		_registry(registry), _window(window), _events(events), _inputs(inputs) {}
	~Systems() {}
	void init(float screenWidth, float screenHeight) { _gameWidth = screenWidth; _gameHeight = screenHeight; }
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
	float _gameWidth, _gameHeight;
	entt::registry* _registry;
	EventManager* _events;
	InputManager* _inputs;
	Program* _program;
	SpriteBatch batch;
	sf::RenderWindow* _window;
};

