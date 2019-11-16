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



MainGame::MainGame() : _window(sf::VideoMode(750, 750), "ECStroids"), _gameDims(750, 750),
_gameState(GameState::PLAY), _fpsLimiter(200.0f), _fps(120.0f),
_events(&_registry), _systems(&_registry, &_window, &_events, &_inputManager) {}

MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	AssetManager::createPlayer();
	gameLoop();
}

void MainGame::initSystems() {
	_view.setSize(sf::Vector2f(_gameDims.x, _gameDims.y));
	_view.setCenter(sf::Vector2f(_gameDims.x, _gameDims.y) / 2.0f);
	_view.zoom(_window.getSize().x / _gameDims.y);
	_window.setView(_view);
	_systems.init(_gameDims.x, _gameDims.y);

	//Shaders
	//_shader.loadFromFile("Shdaers/textureShading.frag", sf::Shader::Fragment);
	//_shader.setParameter("texture", sf::Shader::CurrentTexture);

	AssetManager::init(&_registry, _gameDims);
	//_batch.init();
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
		processInput();
		_systems.checkInput();
		_systems.moveEntities();
		drawGame();
		if (_events.processEvents(1 / _fps)) {
			break;
		}
		//_systems.checkCollisions();
		static unsigned int loop = 0;
		if (loop % 10 == 0) {
			loop = 1;
			int asteroids = _registry.view<entt::tag<"Enemy"_hs>>().size();
			int bullets = _registry.view<entt::tag<"Player"_hs>>().size();
			_window.setTitle(std::string("ECStroids - FPS: " + std::to_string(_fps) + " Asteroids: " + std::to_string(asteroids) + " Bullets: " + std::to_string(bullets)).c_str());
			//std::cout << _fps << " with " << _registry.view<entt::tag<"Player"_hs>>().size() << " and " << _registry.view<entt::tag<"Enemy"_hs>>().size() << "\n";
		} else {
			loop++;
		}
		_fps = _fpsLimiter.end();
	}
}

void MainGame::processInput() {
	sf::Event event;
	while (_window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			_gameState = GameState::EXIT;
			break;
		case sf::Event::MouseMoved:
			_inputManager.setMouseCoords(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::KeyPressed:
			_inputManager.pressKey(event.key.code);
			break;
		case sf::Event::KeyReleased:
			_inputManager.releaseKey(event.key.code);
			break;
		case sf::Event::MouseButtonPressed:
			_inputManager.pressButton(event.mouseButton.button);
			break;
		case sf::Event::MouseButtonReleased:
			_inputManager.releaseButton(event.mouseButton.button);
			break;
		}
	}
}

void MainGame::drawGame() {
	_window.clear();
	_systems.drawSprites();
	_window.display();
}