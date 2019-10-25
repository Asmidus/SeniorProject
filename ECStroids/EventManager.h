#pragma once
#include "Event.h"
#include "AssetManager.h"
#include "entt/entt.hpp"


//This class is responsible for handling and processing events
class EventManager {
public:
	EventManager(entt::registry* registry, AssetManager* assets) : _registry(registry), _assets(assets) {}
	~EventManager();

	//Register an event to be processed
	void registerEvent(Event event);
	//Process all events in queue
	void processEvents(float dt);

	//These functions each process a specific type of event
	void processCollision(Event event);
	void processButton(Event event);
	void processMove(Event event);
	void processShoot(Event event);
private:
	//The current queue of events to be processed
	std::vector<Event> _events;
	//A pointer to the asset manager (for when an event causes an asset to be created)
	AssetManager* _assets;
	//A pointer to the registry
	entt::registry* _registry;
	float _dt;
};

