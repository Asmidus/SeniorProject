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
	updateAnimations();
	_registry->view<Sprite, Transform>().each(
		[](auto& sprite, auto& transform) {
			TextureManager::Draw(sprite.texIndex, sprite.src, transform.rect, &transform.center, transform.angle);
		});
}

void Systems::updateAnimations() {
	_registry->view<Sprite, Animation>().each(
		[this](auto& sprite, auto& animation) {
			if (animation.active) {
				animation.timeSinceLastFrame += _dt;
				if (animation.timeSinceLastFrame > animation.frameTime[animation.currentAnimation]) {
					unsigned int currFrame = 1 + (sprite.src.x / sprite.src.w);
					if (currFrame > animation.frames[animation.currentAnimation]) {
						currFrame = 0;
					}
					sprite.src.x = sprite.src.w * currFrame;
					sprite.src.y = sprite.src.h * animation.currentAnimation;
					animation.timeSinceLastFrame = 0;
				}
			} else {
				sprite.src.x = 0;
			}
			animation.active = false;
		});
}

void Systems::moveEntities() {
	_registry->view<Transform, Velocity>().each(
		[this](auto& transform, auto& velocity) {
			glm::vec2 deltaVel = glm::vec2(0, 0);
			//if the entity is trying to accelerate in its given direction, apply that acceleration
			if (velocity.currAccel != 0) {
				deltaVel = glm::normalize(velocity.direction) * (velocity.currAccel);
			//if the entity is not trying to accelerate, decelerate it by its deceleration amount
			} else if(glm::length(velocity.currVel) > 0) {
				deltaVel = glm::normalize(velocity.currVel) * -(velocity.decel);
			}
			//apply the change in velocity
			velocity.currVel += (deltaVel * _dt);
			//if the entity's net speed is faster than the max, cap it
			if (glm::length(velocity.currVel) > velocity.maxSpeed) {
				velocity.currVel = glm::normalize(velocity.currVel) * velocity.maxSpeed;
			//if the entity's net speed is essentially zero, reset the current velocity vector
			} else if (glm::length(velocity.currVel) < 0.01) {
				velocity.currVel = glm::vec2(0, 0);
			}
			//update the transform of the entity
			transform.updatePos(velocity.currVel * 120.0f * _dt);
			transform.angle = glm::angle(velocity.direction, glm::vec2(1, 0))*(180/3.14159);
			//make sure we can get angles larger than 180 degrees
			if (velocity.direction.y < 0) {
				transform.angle = 360 - transform.angle;
			}
			//reset the current acceleration
			velocity.currAccel = 0;
	});
	_registry->view<Transform, ScreenWrap>().each(
		[this](auto & transform, auto & wrap) {
			if (transform.pos.x < -transform.rect.w) {
				transform.pos.x = _screenWidth;
			} else if(transform.pos.x > _screenWidth) {
				transform.pos.x = -transform.rect.w;
			}
			if (transform.pos.y < -transform.rect.h) {
				transform.pos.y = _screenHeight;
			} else if (transform.pos.y > _screenHeight) {
				transform.pos.y = -transform.rect.h;
			}
			transform.rect.x = transform.pos.x;
			transform.rect.y = transform.pos.y;
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
		[this](auto & cooldown) {
			cooldown.decrementCooldowns(_dt);
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

void Systems::checkLifetimes() {
	auto view = _registry->view<Lifetime>();
	for (auto& entity : view) {
		auto& lifetime = _registry->get<Lifetime>(entity);
		lifetime.timeLeft -= _dt;
		if (lifetime.timeLeft <= 0) {
			_registry->destroy(entity);
		}
	}
}