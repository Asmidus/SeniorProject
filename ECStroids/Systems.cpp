#include "Systems.h"
#include "MainGame.h"
#include "Components.h"
#include "entt/entt.hpp"
#include "Sprite.h"
#include "EventManager.h"
#include "InputManager.h"
#include "SpriteBatch.h"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#include <execution>



void Systems::drawSprites() {
	updateAnimations();
	//unsigned int lightNum = 0;
	//for (auto& entity : _registry->group<Light>(entt::get<Transform>)) {
	//	if (lightNum >= 128) break;
	//	auto [light, transform] = _registry->get<Light, Transform>(entity);
	//	glm::vec2 lightPos = light.pos * glm::vec2(transform.rect.w, transform.rect.h);
	//	float angle = transform.angle;
	//	auto center = transform.center * glm::vec2(transform.rect.w, transform.rect.h);
	//	float rotatedX = cos(angle) * (lightPos.x - center.x) - sin(angle) * (lightPos.y - center.y) + center.x + transform.rect.x;
	//	float rotatedY = sin(angle) * (lightPos.x - center.x) + cos(angle) * (lightPos.y - center.y) + center.y + transform.rect.y;
	//	rad[lightNum] = light.radius;
	//	pos[2 * lightNum] = rotatedX;
	//	pos[2 * lightNum + 1] = rotatedY;
	//	col[3 * lightNum] = light.color.r;
	//	col[3 * lightNum + 1] = light.color.g;
	//	col[3 * lightNum + 2] = light.color.b;
	//	++lightNum;
	//}
	//batch->begin(GlyphSortType::BACK_TO_FRONT);
	batch.begin(GlyphSortType::BTF_TEXTURE);
	_registry->view<Sprite>().each([this](auto& sprite) {
		//TextureManager::Draw(sprite.texture, sprite.src, transform.rect, &transform.center, transform.angle, sprite.color);
		batch.draw(sprite);
		//glm::vec4 t = glm::vec4(transform.rect.x, transform.rect.y, transform.rect.w, transform.rect.h);
		//glm::vec4 u = sprite.getUV();
		//if (transform.angle) {
		//	batch->draw(t, u, sprite.texture, transform.z, sprite.color, transform.angle, transform.center);
		//} else {
		//	batch->draw(t, u, sprite.texture, transform.z, sprite.color);
		//}

		//auto text = _registry->try_get<Text>(entity);
		//if (text) {
		//	text->dest.x = transform.rect.x + text->offset.x;
		//	text->dest.y = transform.rect.y + text->offset.y;
		//	TextureManager::DrawText(text->texture, text->dest);
		//}
	});
	batch.end();
	batch.renderBatch(*_window);
	//batch->end();
	//batch->renderBatch();
	//_registry->group<>(entt::get<Text>, entt::exclude<Transform>).each(
	//	[this](auto entity, auto& text) {
	//		TextureManager::DrawText(text.texture, text.dest);
	//	});
}

void Systems::updateAnimations() {
	//_registry->group<>(entt::get<Sprite, Animation>).each(
	//	[this](auto& sprite, auto& animation) {
	//		if (animation.active) {
	//			animation.timeSinceLastFrame += _dt;
	//			if (animation.timeSinceLastFrame > animation.frameTime[animation.currentAnimation]) {
	//				unsigned int currFrame = 1 + (sprite.src.x / sprite.src.w);
	//				if (currFrame > animation.frames[animation.currentAnimation]) {
	//					currFrame = 0;
	//				}
	//				sprite.src.x = sprite.src.w * currFrame;
	//				sprite.src.y = sprite.src.h * animation.currentAnimation;
	//				animation.timeSinceLastFrame = 0;
	//			}
	//		} else {
	//			sprite.src.x = 0;
	//		}
	//	});
}

void Systems::moveEntities() {
	auto group = _registry->group<Sprite, Velocity>();
	std::for_each(std::execution::par_unseq, group.begin(), group.end(), [=](auto entity) {
		auto [sprite, velocity] = group.get<Sprite, Velocity>(entity);
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
		}
		if (velocity.angular) {
			velocity.direction = glm::rotate<float>(velocity.direction, velocity.angular * _dt);
			sprite.rotate(velocity.angular * _dt * 180/3.14159);
			glm::normalize(velocity.direction);
			//transform.angle = glm::angle(velocity.direction, glm::vec2(1, 0));
			//make sure we can get angles larger than 180 degrees
			//if (velocity.direction.y < 0) transform.angle *= -1;
		}
		//update the transform of the entity
		auto offset = velocity.currVel * 120.0f * _dt;
		sprite.move(offset.x, offset.y);
		//transform.updatePos(velocity.currVel * 120.0f * _dt);
	});
	//_registry->view<Transform>().each(
	//	[this](auto& transform) {
	//		if (transform.rect.x < -transform.rect.w) {
	//			transform.rect.x = _gameWidth;
	//		} else if(transform.rect.x > _gameWidth) {
	//			transform.rect.x = -transform.rect.w;
	//		}
	//		if (transform.rect.y < -transform.rect.h) {
	//			transform.rect.y = _gameHeight;
	//		} else if (transform.rect.y > _gameHeight) {
	//			transform.rect.y = -transform.rect.h;
	//		}
	//	});
}

void Systems::checkCollisions() {
	std::shared_ptr<std::mutex> m = std::make_shared<std::mutex>();

	auto enemies = _registry->group<entt::tag<"Enemy"_hs>>(entt::get<Sprite, Collider>);
	auto group = _registry->group<entt::tag<"Player"_hs>>(entt::get<Sprite, Collider>);

	//See if a simple 4-tile quad collision algorithm would be more efficient than brute force
	if (true) {
		static const sf::FloatRect quadDims[4] = {
			sf::FloatRect(0, 0, _gameWidth/2, _gameHeight/2),
			sf::FloatRect(_gameWidth / 2, 0, _gameWidth / 2, _gameHeight / 2),
			sf::FloatRect(0, _gameHeight / 2, _gameWidth / 2, _gameHeight / 2),
			sf::FloatRect(_gameWidth / 2, _gameHeight / 2, _gameWidth / 2, _gameHeight / 2)
		};
		static const std::vector<unsigned int> quads = { 0, 1, 2, 3 };
		auto colliders = _registry->group<Collider>(entt::get<Sprite>);
		std::for_each(std::execution::par_unseq, quads.begin(), quads.end(), [=](auto i) {
			std::vector<const entt::entity*> quadrants;
			auto test = &quadrants;
			//For some reason the debug compiler needs quadDims to be used in this lambda so it can be used in the next one
			//hopefully this line is just optimized out in release
			quadDims;
			std::shared_ptr<std::mutex> m2 = std::make_shared<std::mutex>();
			for (auto& entity : colliders) {
				auto& sprite = colliders.get<Sprite>(entity);
				if (quadDims[i].intersects(sprite.getGlobalBounds())) {
					std::lock_guard<std::mutex> lock{ *m2 };
					(*test).push_back(&entity);
				}
			}
			for (unsigned int j = 0; j < quadrants.size(); j++) {
				auto entity1 = quadrants[j];
				//if (_registry->has<entt::tag<"Player"_hs>>(*entity1)) {
					auto [sprite1, col1] = _registry->get<Sprite, Collider>(*entity1);
					for (unsigned int k = j+1; k < quadrants.size(); k++) {
						auto entity2 = quadrants[k];
						//if (_registry->has<entt::tag<"Enemy"_hs>>(*entity2)) {
							auto [sprite2, col2] = _registry->get<Sprite, Collider>(*entity2);
							if (col1.circular) {
								glm::vec2 e1Pos = glm::vec2(sprite1.getPosition().x, sprite1.getPosition().y);
								glm::vec2 e2Pos = glm::vec2(sprite2.getPosition().x, sprite2.getPosition().y);
								if (glm::length(e1Pos - e2Pos) < col1.radius + col2.radius) {
									auto cooldowns1 = _registry->try_get<Cooldown>(*entity1);
									auto cooldowns2 = _registry->try_get<Cooldown>(*entity2);
									if ((!cooldowns1 || cooldowns1->trigger(Event::Type::collision)) &&
										(!cooldowns2 || cooldowns2->trigger(Event::Type::collision))) {
										std::lock_guard<std::mutex> lock{ *m };
										_events->registerEvent(Event(Event::Type::collision, { *entity1, *entity2 }));
									}
								}
							}
						//}
					}
				//}
			}
		});
	} else {
		group.each([=](auto& entity1, auto tag, auto& sprite1, auto& col1) {
			std::for_each(std::execution::par_unseq, enemies.begin(), enemies.end(), [=](auto entity2) {
				auto [sprite2, col2] = enemies.get<Sprite, Collider>(entity2);
				if (col1.circular) {
					glm::vec2 e1Pos = glm::vec2(sprite1.getPosition().x, sprite1.getPosition().y);
					glm::vec2 e2Pos = glm::vec2(sprite2.getPosition().x, sprite2.getPosition().y);
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
}

void Systems::checkInput() {
	//decrement any cooldowns for all entities before checking input
	_registry->view<Cooldown>().each(
		[this](auto & cooldown) {
			cooldown.decrementCooldowns(_dt);
		});

	//For all mouse events, find every mouse listener
	//and find out what event is tied to the button that is pressed and queue the event
	auto mouseView = _registry->view<MouseListener>();
	for (auto entity : mouseView) {
		for (auto button : _inputs->getPressedButtons()) {
			if (mouseView.get(entity).map.find(button) != mouseView.get(entity).map.end()) {
				auto cooldowns = _registry->try_get<Cooldown>(entity);
				auto eventType = mouseView.get(entity).map[button];
				if (cooldowns && !cooldowns->trigger(eventType)) {
					continue;
				}
				auto pos = _window->mapPixelToCoords(sf::Mouse::getPosition(*_window), _window->getView());
				_events->registerEvent(Event(eventType, entity, glm::vec2(pos.x, pos.y)));
			}
		}
	}

	_registry->view<KeyListener>().each([=](auto& listener) {
		for (auto key : _inputs->getPressedKeys()) {
			if (listener.map.find(key) != listener.map.end()) {
				auto action = listener.map[key];
				if (listener.enabled[key]) {
					listener.enabled[key] = action(true);
				}
			}
		}
		for (auto key : _inputs->getReleasedKeys()) {
			if (listener.map.find(key) != listener.map.end()) {
				auto action = listener.map[key];
				action(false);
				listener.enabled[key] = true;
			}
		}
	});
	_inputs->refresh();
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