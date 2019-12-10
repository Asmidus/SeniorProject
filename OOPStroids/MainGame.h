// OOPstroids
#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include "InputManager.h"
#include "Timing.h"
#include "Window.h"

#include <list>
//#include "AssetManager.h"
//#include "EventManager.h"
//#include "Systems.h"

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
	void wrapping(float a, float b, float& fa, float& fb);
	void processInput();
	void drawGame();
	Window _window;
	SDL_Renderer* _renderer;
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	GameState _gameState;
	InputManager _inputManager;
	FPSLimiter _fpsLimiter;
	//entt::registry _registry;
	//EventManager _events;
	//Systems _systems;

	float _fps;
	float _frameTime;
};