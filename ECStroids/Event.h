#pragma once
#include "entt/entt.hpp"

//Events are essentially actions that are triggered from something.
//They can occur when two objects collide or when the user presses a button
class Event {
public:
	enum Type {
		collision,
		button,
		moveUp,
		moveDown,
		moveRight,
		moveLeft,
		shootBullet
	};
	Event();
	Event(const Type& type, const std::vector<entt::entity>& entities) {
		_type = type;
		_entities = entities;
	}
	Event(const Type& type, const entt::entity& entity) {
		_type = type;
		_entities = { entity };
	}
	~Event();

	Type type() { return _type; }
	std::vector<entt::entity> entities() { return _entities; }
private:
	//the list of entities effected by the event
	std::vector<entt::entity> _entities;
	//the type of event
	Type _type;
};