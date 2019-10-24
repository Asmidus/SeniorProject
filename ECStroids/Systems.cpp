#include "Systems.h"
#include "MainGame.h"
#include "Components.h"
#include "entt/entt.hpp"
#include "Sprite.h"
#include "EventManager.h"
#include "InputManager.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>



void Systems::drawSprites() {
	_registry->view<Sprite, Transform>().each(
		[](auto& sprite, auto& transform) {
			//if (transform.angle == 0) {
			//	TextureManager::Draw(sprite.texture, sprite.src, sprite.dest);
			//	//if (sprite.scrolling) {
			//	//	TextureManager::Draw(sprite.texture, sprite.src, sprite.scrollDest);
			//	//}
			//} else {
			TextureManager::Draw(sprite.texIndex, sprite.src, transform.rect, &transform.center, transform.angle);
			//}
		});
}

void Systems::moveEntities() {
	_registry->view<Transform, Velocity>().each(
		[](auto& transform, auto& velocity) {
			glm::vec2 deltaVel = glm::vec2(0, 0);
			//if the entity is trying to accelerate in its given direction, apply that acceleration
			if (velocity.currAccel != 0) {
				deltaVel = glm::normalize(velocity.direction) * (velocity.currAccel / 60);
			//if the entity is not trying to accelerate, decelerate it by its deceleration amount
			} else if(glm::length(velocity.currVel) > 0) {
				deltaVel = glm::normalize(velocity.currVel) * -(velocity.decel / 60);
			}
			//apply the change in velocity
			velocity.currVel += deltaVel;
			//if the entity's net speed is faster than the max, cap it
			if (glm::length(velocity.currVel) > velocity.maxSpeed) {
				velocity.currVel = glm::normalize(velocity.currVel) * velocity.maxSpeed;
			//if the entity's net speed is essentially zero, reset the current velocity vector
			} else if (glm::length(velocity.currVel) < 0.01) {
				velocity.currVel = glm::vec2(0, 0);
			}
			//update the transform of the entity
			transform.updatePos(velocity.currVel);
			transform.angle = glm::angle(velocity.direction, glm::vec2(1, 0))*(180/3.14159);
			//make sure we can get angles larger than 180 degrees
			if (velocity.direction.y < 0) {
				transform.angle = 360 - transform.angle;
			}
			//reset the current acceleration
			velocity.currAccel = 0;
	});
	//auto view = _registry->view<Position>();
	//std::vector<entt::entity> a;
	//int count = 0;
	//for (auto entity : view) {
	//	a.push_back(entity);
	//}
	//_events->registerEvent(Event(Event::Type::collision, a));
}

void Systems::checkInput() {
	//decrement any cooldowns for all entities before checking input
	_registry->view<Cooldown>().each(
		[](auto & cooldown) {
			cooldown.decrementCooldowns();
		});
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
			if (view.get(entity).map.find(key) != view.get(entity).map.end()) {
				auto cooldowns = _registry->try_get<Cooldown>(entity);
				auto eventType = view.get(entity).map[key];
				if (cooldowns && !cooldowns->trigger(eventType)) {
					continue;
				}
				_events->registerEvent(Event(eventType, entity));
			}
		}
	}
	auto keyView = _registry->view<KeyListener>();
	for (auto key : _inputs->getPressedKeys()) {
		for (auto entity : keyView) {
			if (keyView.get(entity).map.find(key) != keyView.get(entity).map.end()) {
				_events->registerEvent(Event(keyView.get(entity).map[key], entity));
			}
		}
	}
}