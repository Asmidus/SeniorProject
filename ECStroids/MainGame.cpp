#include "MainGame.h"
#include <iostream>
#include "Errors.h"
#include "InputManager.h"
#include <string>
#include <random>
#include <time.h>
#include <string>
#include "Transform.h"
#include "Velocity.h"
#include "Sprite.h"
#include "TextureManager.h"

MainGame::MainGame() : _screenWidth(800), _screenHeight(800),
_gameState(GameState::PLAY), _fpsLimiter(120.0f), _fps(30.0f),
_assets(&_registry), _events(&_registry, &_assets), _systems(&_registry, &_events, &_inputManager) {}


MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	_assets.createPlayer();
	gameLoop();
}

void MainGame::initSystems() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	_renderer = _window.create("ECStroids", _screenWidth, _screenHeight, 0);
	_systems.init(_screenWidth, _screenHeight);
	TextureManager::init(_renderer);
	srand(time(0));
}

void MainGame::initLevel() {

}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();
		_systems.updateDelta(1 / _fps);
		_systems.checkLifetimes();
		_systems.checkInput();
		_systems.moveEntities();
		_systems.checkCollisions();
		_events.processEvents(1 / _fps);
		checkSpawnAsteroids();
		drawGame();
		static unsigned int loop = 0;
		if (loop % 200 == 0) {
			loop = 1;
			std::cout << _fps << " with " << _registry.view<entt::tag<"Player"_hs>>().size() << " and " << _registry.view<entt::tag<"Enemy"_hs>>().size() << std::endl;
			if (_registry.view<entt::tag<"Enemy"_hs>>().size() == 1) {
				for (auto entity : _registry.view < entt::tag<"Enemy"_hs>>()) {
					std::cout << _registry.get<Transform>(entity).pos.x << " " << _registry.get<Transform>(entity).pos.y << std::endl;
				}
			}
		} else {
			loop++;
		}
		_fps = _fpsLimiter.end();
	}
}

void MainGame::checkSpawnAsteroids() {
	static unsigned int asteroidCount = 3;
	auto view = _registry.view<entt::tag<"Enemy"_hs>>();
	if (view.empty()) {
		for (int i = 0; i < asteroidCount; i++) {
			bool topSpawn = rand() % 2;
			if (topSpawn) {
				_assets.createAsteroid(rand() % _screenWidth, -99 + rand()%2 * (_screenHeight + 99));
			} else {
				_assets.createAsteroid(-99 + rand() % 2 * (_screenWidth + 99), rand() % _screenHeight);
			}
		}
		asteroidCount++;
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