#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include "InputManager.h"
#include "Timing.h"
#include "Window.h"
#include "entt/entt.hpp"
#include <list>
#include "AssetManager.h"
#include "EventManager.h"
#include "Systems.h"

enum class GameState {
	PLAY,
	EXIT
};

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initLevel();
	void gameLoop();
	void processInput();
	void drawGame();
	X::Window _window;
	SDL_Renderer* _renderer;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	InputManager _inputManager;
	FPSLimiter _fpsLimiter;
	entt::registry _registry;
	AssetManager _assets;
	EventManager _events;
	Systems _systems;

	float _fps;
	float _frameTime;
	float _maxFPS;
};