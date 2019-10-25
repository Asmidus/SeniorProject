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
#include "TextureManager.h"

MainGame::MainGame() : _screenWidth(800), _screenHeight(600),
_gameState(GameState::PLAY), _fpsLimiter(120.0f), 
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
	_renderer = _window.create("Test Engine", _screenWidth, _screenHeight, 0);
	TextureManager::init(_renderer);
	srand(time(0));
}

void MainGame::initLevel() {

}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();
		drawGame();
		//processInput();
		_systems.checkInput();
		_systems.moveEntities();
		_events.processEvents();
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
	//static SDL_Texture* tex1 = TextureManager::LoadTexture("media/PlayerShip.png");
	//static SDL_Texture* tex2 = TextureManager::LoadTexture("media/Projectile.png");
	//SDL_Rect s;
	//s.x = 0;
	//s.y = 0;
	//s.w = 50;
	//s.h = 50;
	//TextureManager::Draw(tex1, s, s);
	//TextureManager::Draw(tex2, s, s);
	_systems.drawSprites();
	SDL_RenderPresent(_renderer);
	//_window.swapBuffer();
}