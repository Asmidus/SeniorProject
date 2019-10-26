#pragma once
#include "entt/entt.hpp"
class AssetManager
{
public:
	AssetManager(entt::registry* registry) : _registry(registry) {}
	~AssetManager();

	entt::entity createPlayer();
	entt::entity createBullet(entt::entity& shooter, bool tracking = false);
	entt::entity createAsteroid();
private:
	entt::registry* _registry;
};