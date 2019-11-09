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
#include <algorithm>
#include <execution>
#include "SpriteBatch.h"

void Systems::drawSprites(SpriteBatch* batch) {
	updateAnimations();
	batch->begin();
	_registry->group<Sprite, Transform>().each(
		[batch](auto entity, auto& sprite, auto& transform) {
			//TextureManager::Draw(sprite.texture, sprite.src, transform.rect, &transform.center, transform.angle, sprite.color);
			glm::vec4 t = glm::vec4(transform.rect.x, transform.rect.y, transform.rect.w, transform.rect.h);
			glm::vec4 u = sprite.getUV();
			if (transform.angle) {
				batch->draw(t, u, sprite.texture, 0, sprite.color, transform.angle);
			} else {
				batch->draw(t, u, sprite.texture, 0, sprite.color);
			}
			//auto text = _registry->try_get<Text>(entity);
			//if (text) {
			//	text->dest.x = transform.rect.x + text->offset.x;
			//	text->dest.y = transform.rect.y + text->offset.y;
			//	TextureManager::DrawText(text->texture, text->dest);
			//}
		});
	batch->end();
	batch->renderBatch();
	//_registry->group<>(entt::get<Text>, entt::exclude<Transform>).each(
	//	[this](auto entity, auto& text) {
	//		TextureManager::DrawText(text.texture, text.dest);
	//	});
}

void Systems::updateAnimations() {
	_registry->group<>(entt::get<Sprite, Animation>).each(
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
	auto group = _registry->group<Sprite, Transform, Velocity>();
	std::for_each(std::execution::par_unseq, group.begin(), group.end(), [=](auto entity) {
		auto [sprite, transform, velocity] = group.get<Sprite, Transform, Velocity>(entity);
		if (!velocity.constant) {
			glm::vec2 deltaVel = glm::vec2(0, 0);
			//if the entity is trying to accelerate in its given direction, apply that acceleration
			if (velocity.currAccel != 0) {
				deltaVel = glm::normalize(velocity.direction) * (velocity.currAccel);
				//if the entity is not trying to accelerate, decelerate it by its deceleration amount
			} else if (velocity.decel && glm::length(velocity.currVel) > 0) {
				deltaVel = glm::normalize(velocity.currVel) * -(velocity.decel);
			}
			//apply the change in velocity
			velocity.currVel += (deltaVel * _dt);
			//if the entity's net speed is faster than the max, cap it
			if (glm::length(velocity.currVel) > velocity.maxSpeed) {
				velocity.currVel = glm::normalize(velocity.currVel) * velocity.maxSpeed;
				//if the entity's net speed is essentially zero, reset the current velocity vector
			} else if (velocity.currAccel == 0 && glm::length(velocity.currVel) < 0.01) {
				velocity.currVel = glm::vec2(0, 0);
			}
			//reset the current acceleration
			velocity.currAccel = 0;
		}
		//update the transform of the entity
		transform.updatePos(velocity.currVel * 120.0f * _dt);
	});
	_registry->view<Transform>().each(
		[this](auto& transform) {
			if (transform.rect.x < -transform.rect.w) {
				transform.rect.x = _screenWidth;
			} else if(transform.rect.x > _screenWidth) {
				transform.rect.x = -transform.rect.w;
			}
			if (transform.rect.y < -transform.rect.h) {
				transform.rect.y = _screenHeight;
			} else if (transform.rect.y > _screenHeight) {
				transform.rect.y = -transform.rect.h;
			}
		});
}

void Systems::checkCollisions() {
	//TODO Brandon's collision algo
	std::shared_ptr<std::mutex> m = std::make_shared<std::mutex>();
	auto enemies = _registry->group<entt::tag<"Enemy"_hs>>(entt::get<Transform, Collider>);
	auto group = _registry->view<entt::tag<"Player"_hs>, Transform, Collider>();
	for (entt::entity entity : group) {
		Collider& collider = _registry->get<Collider>(entity);
	}
	_registry->group<entt::tag<"Player"_hs>>(entt::get<Transform, Collider>).each(
		[=](auto& entity1, auto tag1, auto & trans1, auto & col1) {
			std::for_each(std::execution::par_unseq, enemies.begin(), enemies.end(), [=](auto entity2) {
				auto [trans2, col2] = enemies.get<Transform, Collider>(entity2);
				if (col1.circular) {
					glm::vec2 e1Pos = glm::vec2(trans1.center.x + trans1.rect.x, trans1.center.y + trans1.rect.y);
					glm::vec2 e2Pos = glm::vec2(trans2.center.x + trans2.rect.x, trans2.center.y + trans2.rect.y);
					if (glm::length(e1Pos - e2Pos) < col1.radius + col2.radius) {
						auto cooldowns1 = _registry->try_get<Cooldown>(entity1);
						auto cooldowns2 = _registry->try_get<Cooldown>(entity2);
						if ((!cooldowns1 || cooldowns1->trigger(Event::Type::collision)) &&
							(!cooldowns2 || cooldowns2->trigger(Event::Type::collision))) {
							std::lock_guard<std::mutex> lock{ *m };
							_events->registerEvent(Event(Event::Type::collision, { entity1, entity2 }));
						}
					}
				}
			});
		});
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
	auto mouseView = _registry->view<MouseListener>();
	for (auto key : _inputs->getPressedKeys()) {
		for (auto entity : mouseView) {
			if (mouseView.get(entity).map.find(key) != mouseView.get(entity).map.end()) {
				auto cooldowns = _registry->try_get<Cooldown>(entity);
				auto eventType = mouseView.get(entity).map[key];
				if (cooldowns && !cooldowns->trigger(eventType)) {
					continue;
				}
				_events->registerEvent(Event(eventType, entity, _inputs->getMouseCoords()));
			}
		}
	}
	auto keyView = _registry->view<KeyListener>();
	for (auto key : _inputs->getPressedKeys()) {
		for (auto entity : keyView) {
			if (keyView.get(entity).map.find(key) != keyView.get(entity).map.end()) {
				auto cooldowns = _registry->try_get<Cooldown>(entity);
				auto eventType = keyView.get(entity).map[key];
				if (cooldowns && !cooldowns->trigger(eventType)) {
					continue;
				}
				_events->registerEvent(Event(keyView.get(entity).map[key], entity));
			}
		}
	}
}

void Systems::checkLifetimes() {
	_registry->view<Lifetime>().each(
		[this](auto & entity, auto & lifetime) {
			lifetime.timeLeft -= _dt;
			if (lifetime.timeLeft <= 0) {
				_registry->destroy(entity);
			}
		});
}

void Systems::spawnAsteroids() {
	_registry->view<AsteroidSpawner>().each(
		[this](auto& entity, auto& spawner) {
			if (_registry->view<entt::tag<"Enemy"_hs>>().empty()) {
				for (int i = 0; i < spawner.numAsteroids; i++) {
					AssetManager::createAsteroid(spawner.speedRange, spawner.sizeRange);
				}
				spawner.numAsteroids += spawner.increment;
			}
		});
}