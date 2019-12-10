// OOPstroids

#include "MainGame.h"
#include <iostream>
#include "InputManager.h"
#include <string>
#include <random>
#include <time.h>
#include <string>
#include "TextureManager.h"
#include "Asteroid.h"

MainGame::MainGame() : _screenWidth(800), _screenHeight(600),
_gameState(GameState::PLAY), _fpsLimiter(120.0f), _fps(120.0f) {}
//_events(&_registry), _systems(&_registry, &_events, &_inputManager) {}


MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	//AssetManager::createMenu();
	gameLoop();
}

void MainGame::initSystems() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
	//TTF_Init();
	_renderer = _window.create("OOPstroids", _screenWidth, _screenHeight, 0);
	//_systems.init(_screenWidth, _screenHeight);
	//TextureManager::init(_renderer);
	//AssetManager::init(&_registry, &_screenWidth, &_screenHeight);
	srand(time(0));
}

void MainGame::initLevel() {

}

// allows asteroids to wrap around the screen
void MainGame::wrapping(float a, float b, float& fa, float& fb) {
	fa = a;
	fb = b;
	if (a < 0.0f) fa = a + (float)_screenWidth;
	if (a >= (float)_screenWidth) fa = a  - (float)_screenWidth;
	if (b < 0.0f) fb = b + (float)_screenHeight;
	if (b >= (float)_screenHeight) fb = b - (float)_screenHeight;
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();

		if (Asteroid.empty()) {
			// spawn asteroids
			std::vector<Asteroid> a;
			Asteroid newAsteroid(20.0f, 20.0f, 8.0f, -6.0f, (int)16);

			for (int i = 0; i < 3; i++) {
				a.push_back(newAsteroid);
			}
		}

		/*_systems.updateDelta(1 / _fps);
		_systems.checkLifetimes();
		_systems.spawnAsteroids();
		_systems.checkInput();
		_systems.moveEntities(); 
		if (_events.processEvents(1 / _fps)) {
			break;
		}*/
		drawGame();
		//_systems.checkCollisions();
		static unsigned int loop = 0;
		if (loop % int(_fps) == 0) {
			loop = 1;
			SDL_SetWindowTitle(_window.get(), std::string("OOPstroids - FPS: " + std::to_string(_fps)).c_str());
			//std::cout << _fps << " with " << _registry.view<entt::tag<"Player"_hs>>().size() << " and " << _registry.view<entt::tag<"Enemy"_hs>>().size() << "\n";
		}
		else {
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
	//_systems.drawSprites();
	SDL_RenderPresent(_renderer);
}