#pragma once
#include "entt/entt.hpp"
class AssetManager
{
public:
	AssetManager(entt::registry* registry) : _registry(registry) {}
	~AssetManager();

	void createPlayer();
private:
	entt::registry* _registry;
};