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
_gameState(GameState::PLAY), _fpsLimiter(10000.0f), _fps(120.0f),
_events(&_registry), _systems(&_registry, &_events, &_inputManager) {}


MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	AssetManager::createAsteroidSpawner();
	gameLoop();
}

static GLuint a, b;

void MainGame::initSystems() {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	//SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
	TTF_Init();
	_window.create("ECStroids", _screenWidth, _screenHeight, 0);
	//glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	_systems.init(_screenWidth, _screenHeight);
	_camera.init(_screenWidth, _screenHeight);

	//Shaders
	_program.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_program.addAttribute("vertexPosition");
	_program.addAttribute("vertexColor");
	_program.addAttribute("vertexUV");
	_program.link();
	_program.use();
	a = _program.getUniformLocation("mySampler");
	b = _program.getUniformLocation("P");

	AssetManager::init(&_registry, &_screenWidth, &_screenHeight);
	_batch.init();
	srand(time(0));
}

void MainGame::initLevel() {

}

void MainGame::gameLoop() {
	_camera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();
		_systems.updateDelta(1 / _fps);
		_systems.checkLifetimes();
		_systems.spawnAsteroids();
		_systems.checkInput();
		_camera.update();
		_systems.moveEntities();
		if (_events.processEvents(1 / _fps)) {
			break;
		}
		drawGame();
		_systems.checkCollisions();
		static unsigned int loop = 0;
		if (loop % int(_fps) == 0) {
			loop = 1;
			SDL_SetWindowTitle(_window.get(), std::string("ECStroids - FPS: " + std::to_string(_fps)).c_str());
			//std::cout << _fps << " with " << _registry.view<entt::tag<"Player"_hs>>().size() << " and " << _registry.view<entt::tag<"Enemy"_hs>>().size() << "\n";
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
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Make sure the shader uses texture 0_program.getUniformLocation("mySampler"), _program.getUniformLocation("P")
	glUniform1i(a, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(b, 1, GL_FALSE, &projectionMatrix[0][0]);
	_systems.drawSprites(&_batch);
	_window.swapBuffer();
}