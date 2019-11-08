#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <list>
#include "InputManager.h"
#include "Timing.h"
#include "Window.h"
#include "entt/entt.hpp"
#include "AssetManager.h"
#include "EventManager.h"
#include "Systems.h"
#include "Program.h"

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
	Window _window;
	Program _program;
	SDL_Renderer* _renderer;
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	GameState _gameState;
	InputManager _inputManager;
	FPSLimiter _fpsLimiter;
	entt::registry _registry;
	EventManager _events;
	Systems _systems;

	float _fps;
	float _frameTime;
};