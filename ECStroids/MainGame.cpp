#include "MainGame.h"
#include <iostream>
#include "InputManager.h"
#include <string>
#include <random>
#include <time.h>
#include <string>
#include "Transform.h"
#include "Velocity.h"
#include "Sprite.h"
#include "TextureManager.h"

MainGame::MainGame() : _screenWidth(800), _screenHeight(600),
_gameState(GameState::PLAY), _fpsLimiter(120.0f), _fps(30.0f),
_events(&_registry), _systems(&_registry, &_events, &_inputManager) {}


MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	AssetManager::createMenu();
	gameLoop();
}

void MainGame::initSystems() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
	TTF_Init();
	_renderer = _window.create("ECStroids", _screenWidth, _screenHeight, 0);
	_systems.init(_screenWidth, _screenHeight);
	TextureManager::init(_renderer);
	AssetManager::init(&_registry, &_screenWidth, &_screenHeight);
	srand(time(0));
}

void MainGame::initLevel() {

}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();
		_systems.updateDelta(1 / _fps);
		_systems.checkLifetimes();
		_systems.spawnAsteroids();
		_systems.checkInput();
		_systems.moveEntities();
		_systems.checkCollisions();
		if (_events.processEvents(1 / _fps)) {
			break;
		}
		drawGame();
		static unsigned int loop = 0;
		if (loop % 200 == 0) {
			loop = 1;
			std::cout << _fps << " with " << _registry.view<entt::tag<"Player"_hs>>().size() << " and " << _registry.view<entt::tag<"Enemy"_hs>>().size() << std::endl;
		} else {
			loop++;
		}
		_fps = _fpsLimiter.end();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void MainGame::drawGame() {
	SDL_RenderClear(_renderer);
	_systems.drawSprites();
	SDL_RenderPresent(_renderer);
}