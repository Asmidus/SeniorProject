#pragma once
#include "entt/entt.hpp"
class Event
{
public:
	enum Type {
		collision,
		button
	};
	Event();
	Event(const Type& type, const std::vector<entt::entity>& entities) {
		_type = type;
		_entities = entities;
	}
	Event(const Type& type, const entt::entity& entity) {
		_type = type;
		_entities = {entity};
	}
	~Event();

	Type type() { return _type; }
	std::vector<entt::entity> entities() { return _entities; }
private:
	std::vector<entt::entity> _entities;
	Type _type;
	
};

