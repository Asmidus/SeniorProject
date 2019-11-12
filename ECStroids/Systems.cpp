#include "Systems.h"
#include "MainGame.h"
#include "Components.h"
#include "entt/entt.hpp"
#include "Sprite.h"
#include "EventManager.h"
#include "InputManager.h"
#include "SpriteBatch.h"
#include "Camera.h"
#include "Program.h"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#include <execution>



void Systems::drawSprites(SpriteBatch* batch) {
	updateAnimations();
	glUniform1i(_program->getUniformLocation("numLights"), _registry->view<Light>().size());
	std::vector<GLfloat> pos;
	std::vector<GLfloat> col;
	for (auto& entity : _registry->group<Light>(entt::get<Transform>)) {
		auto [light, transform] = _registry->get<Light, Transform>(entity);
		glm::vec2 lightPos = transform.center * glm::vec2(transform.rect.w, transform.rect.h);
		lightPos += glm::vec2(transform.rect.x, transform.rect.y);
		pos.push_back(lightPos.x);
		pos.push_back(lightPos.y);
		col.push_back(light.color.r);
		col.push_back(light.color.g);
		col.push_back(light.color.b);
	}
	//_registry->group<Light>(entt::get<Transform>).each([=](auto entity, auto & light, auto & transform) {
	//	glm::vec2 lightPos = transform.center * glm::vec2(transform.rect.w, transform.rect.h);
	//	lightPos += glm::vec2(transform.rect.x, transform.rect.y);
	//	//(*poss).push_back(lightPos.x);
	//	//pos.push_back(lightPos.y);
	//	//col.push_back(light.color.r);
	//	//col.push_back(light.color.g);
	//	//col.push_back(light.color.b);
	//	//glUniform2f(_program->getUniformLocation("lightLoc"), lightPos.x, lightPos.y);
	//	//glUniform3f(_program->getUniformLocation("lightCol"), light.color.r, light.color.g, light.color.b);
	//	glColorMask(false, false, false, false);
	//	glStencilFunc(GL_ALWAYS, 1, 1);
	//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//	auto group = _registry->group<entt::tag<"Occluder"_hs>>(entt::get<Transform>);
	//	for (auto& entity : group) {
	//		auto& occTransform = group.get<Transform>(entity);
	//		std::vector<glm::vec2> vertices = {
	//			glm::vec2(occTransform.rect.x, occTransform.rect.y),
	//			glm::vec2(occTransform.rect.x + occTransform.rect.w, occTransform.rect.y),
	//			glm::vec2(occTransform.rect.x, occTransform.rect.y + occTransform.rect.h),
	//			glm::vec2(occTransform.rect.x + occTransform.rect.w, occTransform.rect.y + occTransform.rect.h)
	//		};
	//		for (int i = 0; i < vertices.size(); i++) {
	//			glm::vec2 currentVertex = vertices[i];
	//			glm::vec2 nextVertex = vertices[(i + 1) % vertices.size()];
	//			glm::vec2 edge = nextVertex - currentVertex;
	//			glm::vec2 normal = glm::vec2(edge.y, -edge.x);
	//			glm::vec2 lightToCurrent = currentVertex - lightPos;
	//			if (glm::dot(normal, lightToCurrent) > 0) {
	//				glm::vec2 point1 = currentVertex + (currentVertex - lightPos);
	//				glm::vec2 point2 = nextVertex + (nextVertex - lightPos);
	//				glBegin(GL_QUADS);
	//				{
	//					glVertex2f(currentVertex.x, currentVertex.y);
	//					glVertex2f(point1.x, point1.y);
	//					glVertex2f(point2.x, point2.y);
	//					glVertex2f(nextVertex.x, nextVertex.y);
	//				} glEnd();
	//			}
	//		}
	//	}

	//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//	glStencilFunc(GL_EQUAL, 0, 1);
	//	glColorMask(true, true, true, true);

	//	//glUniform2f(_program->getUniformLocation("lightLoc"), lightPos.x, lightPos.y);
	//	//glUniform3f(_program->getUniformLocation("lightCol"), light.color.r, light.color.g, light.color.b);

	//	glBegin(GL_QUADS);
	//	{
	//		glVertex2f(0, 0);
	//		glVertex2f(0, _gameHeight);
	//		glVertex2f(_gameWidth, _gameHeight);
	//		glVertex2f(_gameWidth, 0);
	//	} glEnd();

	//	glClear(GL_STENCIL_BUFFER_BIT);
	//});
	static GLint lightLoc = _program->getUniformLocation("lightLoc");
	static GLint lightCol = _program->getUniformLocation("lightCol");
	glUniform2fv(lightLoc, pos.size(), pos.data());
	glUniform3fv(lightCol, col.size(), col.data());
	batch->begin(GlyphSortType::BACK_TO_FRONT);
	_registry->group<Sprite, Transform>().each([batch](auto entity, auto& sprite, auto& transform) {
		//TextureManager::Draw(sprite.texture, sprite.src, transform.rect, &transform.center, transform.angle, sprite.color);
		glm::vec4 t = glm::vec4(transform.rect.x, transform.rect.y, transform.rect.w, transform.rect.h);
		glm::vec4 u = sprite.getUV();
		if (transform.angle) {
			batch->draw(t, u, sprite.texture, transform.z, sprite.color, transform.angle, transform.center);
		} else {
			batch->draw(t, u, sprite.texture, transform.z, sprite.color);
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
				transform.rect.x = _gameWidth;
			} else if(transform.rect.x > _gameWidth) {
				transform.rect.x = -transform.rect.w;
			}
			if (transform.rect.y < -transform.rect.h) {
				transform.rect.y = _gameHeight;
			} else if (transform.rect.y > _gameHeight) {
				transform.rect.y = -transform.rect.h;
			}
		});
}

void Systems::checkCollisions() {
	std::shared_ptr<std::mutex> m = std::make_shared<std::mutex>();

	auto enemies = _registry->group<entt::tag<"Enemy"_hs>>(entt::get<Transform, Collider>);
	auto group = _registry->view<entt::tag<"Player"_hs>, Transform, Collider>();

	//See if a simple 4-tile quad collision algorithm would be more efficient than brute force
	if (enemies.size() * group.size() > (4 * (enemies.size() + group.size()) + enemies.size() / 4 * group.size())) {
		static const glm::vec2 quadDims[4] = { glm::vec2(0, 0), glm::vec2(_gameWidth / 2, 0), glm::vec2(0, _gameHeight / 2), glm::vec2(_gameWidth / 2, _gameHeight / 2) };
		static const std::vector<unsigned int> quads = { 0, 1, 2, 3 };
		auto colliders = _registry->group<Collider>(entt::get<Transform>);
		std::for_each(std::execution::par_unseq, quads.begin(), quads.end(), [=](auto i) {
			std::vector<const entt::entity*> quadrants;
			auto test = &quadrants;
			//For some reason the debug compiler needs quadDims to be used in this lambda so it can be used in the next one
			//hopefully this line is just optimized out in release
			quadDims;
			std::shared_ptr<std::mutex> m2 = std::make_shared<std::mutex>();
			for (auto& entity : colliders) {
				auto& transform = _registry->get<Transform>(entity);
				if (transform.rect.x + transform.rect.w > quadDims[i].x&& transform.rect.x < quadDims[i].x + _gameWidth / 2 &&
					transform.rect.y + transform.rect.h > quadDims[i].y&& transform.rect.y < quadDims[i].y + _gameHeight / 2) {
					std::lock_guard<std::mutex> lock{ *m2 };
					(*test).push_back(&entity);
				}
			}
			for (unsigned int j = 0; j < quadrants.size(); ++j) {
				auto entity1 = quadrants[j];
				if (_registry->has<entt::tag<"Player"_hs>>(*entity1)) {
					auto [trans1, col1] = _registry->get<Transform, Collider>(*entity1);
					for (unsigned int k = 0; k < quadrants.size(); ++k) {
						auto entity2 = quadrants[k];
						if (_registry->has<entt::tag<"Enemy"_hs>>(*entity2)) {
							auto [trans2, col2] = _registry->get<Transform, Collider>(*entity2);
							if (col1.circular) {
								glm::vec2 e1Pos = glm::vec2(trans1.center.x * trans1.rect.w + trans1.rect.x,
															trans1.center.y * trans1.rect.h + trans1.rect.y);
								glm::vec2 e2Pos = glm::vec2(trans2.center.x * trans2.rect.w + trans2.rect.x,
															trans2.center.y * trans2.rect.h + trans2.rect.y);
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
						}
					}
				}
			}
		});
	} else {
		group.each([=](auto& entity1, auto tag, auto& trans1, auto& col1) {
			std::for_each(std::execution::par_unseq, enemies.begin(), enemies.end(), [=](auto entity2) {
				auto [trans2, col2] = enemies.get<Transform, Collider>(entity2);
				if (col1.circular) {
					glm::vec2 e1Pos = glm::vec2(trans1.center.x * trans1.rect.w + trans1.rect.x,
												trans1.center.y * trans1.rect.h + trans1.rect.y);
					glm::vec2 e2Pos = glm::vec2(trans2.center.x * trans2.rect.w + trans2.rect.x,
												trans2.center.y * trans2.rect.h + trans2.rect.y);
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

void Systems::checkInput(Camera* currCam) {
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
				_events->registerEvent(Event(eventType, entity, currCam->convertScreenToWorld(_inputs->getMouseCoords())));
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