#pragma once
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "InputManager.h"
#include "Timing.h"
#include "entt/entt.hpp"
#include "AssetManager.h"
#include "EventManager.h"
#include "Systems.h"
//#include "SpriteBatch.h"

enum class GameState {
	PLAY,
	EXIT
};

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();
	void quitQame() { _gameState = GameState::EXIT; }

private:
	void initSystems();
	void initLevel();
	void gameLoop();
	void processInput();
	void drawGame();
	sf::RenderWindow _window;
	sf::Shader _shader;
	sf::View _view;
	glm::vec2 _gameDims;
	GameState _gameState;
	InputManager _inputManager;
	FPSLimiter _fpsLimiter;
	entt::registry _registry;
	EventManager _events;
	Systems _systems;
	//SpriteBatch _batch;

	float _fps;
	float _frameTime;
};