#include "Systems.h"
#include "MainGame.h"
#include "Components.h"
#include "entt/entt.hpp"
#include "Sprite.h"
#include "EventManager.h"
#include "InputManager.h"
#include <iostream>



void Systems::init(GLint x, GLint y) {
	xOffset = x;
	yOffset = y;
}

void Systems::drawSprites() {
	_registry->view<Sprite, Position>().each(
		[this](auto& sprite, auto& position) {
		sprite.dest.x = position.x;
		sprite.dest.y = position.y;
		if (!sprite.rotated) {
			TextureManager::Draw(sprite.texture, sprite.src, sprite.dest);
			if (sprite.scrolling) {
				TextureManager::Draw(sprite.texture, sprite.src, sprite.scrollDest);
			}
		} else {
			//TextureManager::Draw(sprite.texture, sprite.src, sprite.dest, transform->center, transform->angle);
		}
		});
}

void Systems::moveEntities() {
	//registry.view<Position, Velocity>().each(
	//	[](auto& position, auto& velocity) {
	//		position.x += velocity.dx;
	//		position.y += velocity.dy;
	//	});
	auto view = _registry->view<Position>();
	std::vector<entt::entity> a;
	int count = 0;
	for (auto entity : view) {
		a.push_back(entity);
	}
	//_events->registerEvent(Event(Event::Type::collision, a));
}

void Systems::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_MOUSEMOTION:
			_inputs->setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputs->pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputs->releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputs->pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputs->releaseKey(evnt.button.button);
			break;
		}
	}
	//For all mouse events, find every mouse listener
	//and find out what event is tied to the button that is pressed and queue the event
	auto view = _registry->view<MouseListener>();
	for (auto key : _inputs->getPressedKeys()) {
		for (auto entity : view) {
			_events->registerEvent(Event(view.get(entity).map[key], entity));
		}
	}
	////For all key events, find every key listener
	////and find out what event is tied to the button that is pressed and queue the event
	//auto view = _registry->view<KeyListener>();
	//for (auto entity : view) {
	//	_events->registerEvent(Event(view.get(entity).map[evnt], entity));
	//}
}